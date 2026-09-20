#include <stdio.h>
#include <string.h>

void xorEncrypt(char *message, const char *key) {
    int keyLen = strlen(key);
    for (int i = 0; message[i] != '\0'; i++) {
        message[i] ^= key[i % keyLen];
    }
}

void encryptFile(const char *filename, const char *message, const char *key) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Error opening file for writing.\n");
        return;
    }

    xorEncrypt((char *)message, key);
    fprintf(file, "%s", message);
    fclose(file);
}

void decryptFile(const char *filename, const char *key) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file for reading.\n");
        return;
    }

    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = (char *)malloc(fileSize + 1);
    if (!buffer) {
        printf("Memory allocation failed.\n");
        fclose(file);
        return;
    }

    fread(buffer, 1, fileSize, file);
    buffer[fileSize] = '\0';
    fclose(file);

    xorEncrypt(buffer, key);
    printf("%s\n", buffer);
    free(buffer);
}

void readInput(char* buffer, int size) {
    if (fgets(buffer, size, stdin) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0';
    }
}

int main() {
    char choice[10];
    printf("Enter 'encrypt' or 'decrypt': ");
    //scanf("%s", choice);
    readInput(choice, sizeof(choice));

    if (strcmp(choice, "encrypt") == 0) {
        char message[1024];
        char filename[100];
        char key[100];

        printf("Enter the message to encrypt: ");
        readInput(message, sizeof(message));

        printf("Enter the filename to save encrypted data: ");
        //scanf("%s", filename);
        readInput(filename, sizeof(filename));

        printf("Enter the encryption key: ");
        //scanf("%s", key);
        readInput(key, sizeof(key));

        encryptFile(filename, message, key);
    } else if (strcmp(choice, "decrypt") == 0) {
        char filename[100];
        char key[100];

        printf("Enter the filename to decrypt: ");
        readInput(filename, sizeof(filename));

        printf("Enter the decryption key: ");
        readInput(key, sizeof(key));

        decryptFile(filename, key);
    } else {
        printf("Invalid choice.\n");
    }

    return 0;
}