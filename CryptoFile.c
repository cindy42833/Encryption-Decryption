#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <sys/time.h>

void handleErrors(void)
{
    ERR_print_errors_fp(stderr);
    abort();
}

int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
            unsigned char *iv, unsigned char *ciphertext, int *mode)
{
    EVP_CIPHER_CTX *ctx;

    int len;

    int ciphertext_len;

    /* Create and initialise the context */
    if(!(ctx = EVP_CIPHER_CTX_new()))
        handleErrors();

    /* Initialise the encryption operation. */
    const EVP_CIPHER* (*func_ptr) ();

    /* Set encryption mode */
    switch (*mode) {
    case 0:
        func_ptr = EVP_aes_256_cbc;
        break;
    case 1:
        func_ptr = EVP_aes_256_ctr;
        break;
    case 2:
        func_ptr = EVP_chacha20;
        break;
    }

    if(1 != EVP_EncryptInit_ex(ctx, func_ptr(), NULL, key, iv))
        handleErrors();

    /* Provide the message to be encrypted, and obtain the encrypted output. */
    if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
        handleErrors();
    ciphertext_len = len;

    /*
     * Finalise the encryption. Further ciphertext bytes may be written at
     * this stage.
     */
    if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len))
        handleErrors();
    ciphertext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len;
}

int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
            unsigned char *iv, unsigned char *plaintext, int *mode)
{
    EVP_CIPHER_CTX *ctx;

    int len;

    int plaintext_len;

    /* Create and initialise the context */
    if(!(ctx = EVP_CIPHER_CTX_new()))
        handleErrors();

    /* Initialise the decryption operation. */
    const EVP_CIPHER* (*func_ptr) ();


    /* Set decryption mode */
    switch (*mode) {
    case 0:
        func_ptr = EVP_aes_256_cbc;
        break;
    case 1:
        func_ptr = EVP_aes_256_ctr;
        break;
    case 2:
        func_ptr = EVP_chacha20;
        break;
    }

    if(1 != EVP_DecryptInit_ex(ctx, func_ptr(), NULL, key, iv))
        handleErrors();

    /*
     * Provide the message to be decrypted, and obtain the plaintext output.
     * EVP_DecryptUpdate can be called multiple times if necessary.
     */
    if(1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
        handleErrors();
    plaintext_len = len;

    /*
     * Finalise the decryption. Further plaintext bytes may be written at
     * this stage.
     */
    if(1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len))
        handleErrors();
    plaintext_len += len;

    /* Clean up */
    EVP_CIPHER_CTX_free(ctx);

    return plaintext_len;
}


int main (int argc, char **argv)
{
    /* A 256 bit key */
    unsigned char *key = (unsigned char *)"01234567890123456789012345678901";

    /* A 128 bit IV */
    unsigned char *iv = (unsigned char *)"0123456789012345";

    /* Message to be encrypted */
    FILE *fp;
    int bufferSize = 1024*1024*128;

    unsigned char *plaintext = (unsigned char *) malloc(sizeof(unsigned char) * bufferSize);;

    if(argc < 3) {
        fprintf(stderr, "Usage: ./CryptoFile <filename> <mode: (0) AES256-CBC (1) AES256-CTR (2) ChaCha20>\n");
        exit(0);
    }

    int mode = atoi(argv[2]);

    if(mode > 2) {
        fprintf(stderr, "Mode only has three options! Usage: ./CryptoFile <filename> <mode: (0) AES256-CBC (1) AES256-CTR (2) ChaCha20>\n");
        exit(0);
    }

    errno = 0;
    fp = fopen(argv[1], "r");
    if (errno != 0 ) {
        perror("Error occurred while opening file");
        exit(1);
    }

    fseek(fp, 0, SEEK_SET);
    fread(plaintext, bufferSize, 1, fp);    
    
    /* Buffer for ciphertext. */
    unsigned char *ciphertext = (unsigned char *) malloc(sizeof(unsigned char) * bufferSize);

    /* Buffer for the decrypted text */
    unsigned char *decryptedtext = (unsigned char *) malloc(sizeof(unsigned char) * bufferSize);

    int decryptedtext_len, ciphertext_len;

    /* Encrypt the plaintext */
    struct timeval start;
    struct timeval end;

    gettimeofday(&start, NULL);
    ciphertext_len = encrypt (plaintext, strlen ((char *)plaintext), key, iv,
                              ciphertext, &mode);
    gettimeofday(&end, NULL);

    /* Decrypt the ciphertext */
    decryptedtext_len = decrypt(ciphertext, ciphertext_len, key, iv,
                                decryptedtext, &mode);

    /* Add a NULL terminator. We are expecting printable text */
    decryptedtext[decryptedtext_len] = '\0';

    char outFilename[20] = "output";
    char *ext = strrchr(argv[1], '.');
    FILE *output;
    errno = 0;

    /* If original file has extension, concat it to output filename */
    if(ext) {
        strncat(outFilename, ext, strlen(ext));
    }

    output = fopen(outFilename, "w");
    if (errno != 0 ) {
        perror("Error occurred while opening file");
        exit(1);
    }

    fwrite(decryptedtext, decryptedtext_len, 1, output);
    double diff =  end.tv_sec-start.tv_sec + (end.tv_usec-start.tv_usec)/1000000.0;
    printf("Run time: %lf sec\n", diff);
    printf("Speed %lf bytes/sec\n", strlen(plaintext) / diff);
    return 0;
}