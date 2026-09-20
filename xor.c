
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void xorEncrypt(char *message, size_t messageLen, const char *key) {
    size_t keyLen = strlen(key);

    for (size_t i = 0; i < messageLen; i++) {
        message[i] ^= key[i % keyLen];
    }
}

void encryptFile(const char *filename, const char *message, const char *key) {
    FILE *file = fopen(filename, "wb");

    if (!file) {
        printf("Error opening file for writing.\n");
        return;
    }

    size_t messageLen = strlen(message);
    char *encrypted = malloc(messageLen);

    if (!encrypted) {
        printf("Memory allocation failed.\n");
        fclose(file);
        return;
    }

    memcpy(encrypted, message, messageLen);
    xorEncrypt(encrypted, messageLen, key);

    fwrite(encrypted, 1, messageLen, file);

    free(encrypted);
    fclose(file);

    printf("Message encrypted successfully.\n");
}

void decryptFile(const char *filename, const char *key) {
    FILE *file = fopen(filename, "rb");

    if (!file) {
        printf("Error opening file for reading.\n");
        return;
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    if (fileSize < 0) {
        printf("Error reading file size.\n");
        fclose(file);
        return;
    }

    char *buffer = malloc((size_t)fileSize);

    if (!buffer && fileSize > 0) {
        printf("Memory allocation failed.\n");
        fclose(file);
        return;
    }

    size_t bytesRead = fread(buffer, 1, (size_t)fileSize, file);
    fclose(file);

    xorEncrypt(buffer, bytesRead, key);

    fwrite(buffer, 1, bytesRead, stdout);
    printf("\n");

    free(buffer);
}

void readInput(char *buffer, int size) {
    if (fgets(buffer, size, stdin) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0';
    }
}

int main() {
    char choice[10];

    printf("Enter 'encrypt' or 'decrypt': ");
    readInput(choice, sizeof(choice));

    if (strcmp(choice, "encrypt") == 0) {
        char message[1024];
        char filename[100];
        char key[100];

        printf("Enter the message to encrypt: ");
        readInput(message, sizeof(message));

        printf("Enter the filename to save encrypted data: ");
        readInput(filename, sizeof(filename));

        printf("Enter the encryption key: ");
        readInput(key, sizeof(key));

        if (strlen(key) == 0) {
            printf("Error: Key cannot be empty.\n");
            return 1;
        }

        encryptFile(filename, message, key);

    } else if (strcmp(choice, "decrypt") == 0) {
        char filename[100];
        char key[100];

        printf("Enter the filename to decrypt: ");
        readInput(filename, sizeof(filename));

        printf("Enter the decryption key: ");
        readInput(key, sizeof(key));

        if (strlen(key) == 0) {
            printf("Error: Key cannot be empty.\n");
            return 1;
        }

        decryptFile(filename, key);

    } else {
        printf("Invalid choice.\n");
    }

    return 0;
}
