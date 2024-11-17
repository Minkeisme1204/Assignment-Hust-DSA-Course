#include <iostream>
#define STR_LEN 50

int strcmp(const char *str1, const char *str2) {
    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }
    return *(char *)str1 - *(char *)str2;
}

class Data {
    public:
        char name[STR_LEN]; 
        int size; // Size in MB
        Data (char *name, int size) {
            int i = 0;
            while (name[i] != '\0') {
                this->name[i] = name[i];
                i++;
            }#include <iostream>
#define STR_LEN 50

int strcmp(const char *str1, const char *str2) {
    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }
    return *(char *)str1 - *(char *)str2;
}

class Data {
    public:
        char name[STR_LEN]; 
        int size; // Size in MB
        Data (char *name, int size) {
            int i = 0;
            while (name[i] != '\0') {
                this->name[i] = name[i];
                i++;
            }
            this->name[i] = '\0';
            this->size = size;  // Assign the size to the object.
        }
        ~Data() {}
};

class File {
    public: 
        File *nextFile; 
        Data *data;
        File (char *name, int size) {
            nextFile = NULL;
            data = new Data(name, size);  // Initialize data pointer to nullptr, indicating no data associated with the file yet.
        }
        ~File() {
            if(nextFile != NULL) delete nextFile;
            if(data != NULL) delete data;
        }
}; 

class Folder {
    public:
        File *firstFile;
        File *lastFile;
        char name[STR_LEN];

        Folder(char *name) {
            firstFile = NULL;
            lastFile = NULL;
            int i = 0;
            while (name[i] != '\0') {
                this->name[i] = name[i];
                i++;
            }
            this->name[i] = '\0';
        }

        ~Folder() {
            if(firstFile != NULL) delete firstFile; 
            if(lastFile != NULL) delete lastFile;
        }
        // Function Copy, Delete smallest file, Get size folder, Add files

        void addFile(File *file) {
            if (this->firstFile == NULL) {
                this->firstFile = file;
                this->lastFile = file;
            }
            else {
                this->lastFile->nextFile = file;
                this->lastFile = file;
            }
        }

        void printFolder() {
            File *current = this->firstFile;
            while (current != NULL) {
                std::cout << current->data->name << " (" << current->data->size << " MB)" << std::endl;
                current = current->nextFile;
            }
        }

        void deleteFile(char *name) {
            File *current = this->lastFile;
            File *prev = NULL; 

            if (this->firstFile != NULL and strcmp(this->firstFile->data->name, name) == 0) {
                if (this->firstFile == this->lastFile) this->lastFile = NULL;
                this->firstFile = this->firstFile->nextFile;
                delete current;
                return;
            }

            while (current!= NULL and strcmp(current->data->name, name) != 0) {
                prev = current;
                current = current->nextFile;
            }

            if (current == NULL) return;
            prev->nextFile = current->nextFile;
            if (current == this->lastFile) this->lastFile = prev;
            delete current;
            return;

        }
        // #define DEBUG
        int getSizeFolder() {
            if (this->firstFile == NULL) return 0; 
            File *current = this->firstFile;
            int size = 0; 
            while (current != NULL) {
                size += current->data->size;  // Add size of each file to total size.
                current = current->nextFile;
            }
            return size; 
        }

        File *findFile(char *name) {
            File *current = this->firstFile; 
            if (current == NULL) return NULL; 
            while (strcmp(current->data->name, name) != 0 and current != NULL) {

                #ifdef DEBUG
                std::cout << current->data->name << " " << strlen(current->data->name);
                {   
                    int j = 0;
                    while (current->data->name[j] != '\0') {
                        j++;
                    }  // Remove leading and trailing whitespace from the input string.
                    current->data->name[j - 1] = '\0';  // Remove newline character from the input string.
                    std::cout << j << std::endl;
                }
                
                #endif

                current = current->nextFile;
            }
            if (current == NULL) return NULL;
            return current;
        }

        File* getSmallestFile() {
            File *resFile = this->firstFile;
            File *current = this->firstFile;
            while (current != NULL) {
                if (current->data->size < resFile->data->size) {
                    resFile = current;
                }
                current = current->nextFile;
            }
            return resFile; 
        }
}; 

bool fileCopy(char *filename, Folder *source, Folder *destination) {
    File *file = source->findFile(filename);
    if (file == NULL) return false;
    File *newFile = new File(file->data->name, file->data->size);
    destination->addFile(newFile);
    return true;
}

int transferToUSB(Folder *source, int availableSize) {
    int folderSize = source->getSizeFolder(); 
    while (folderSize > availableSize) {
        File *smallest = source->getSmallestFile(); 
        if (smallest == NULL) return -1; 
        folderSize -= smallest->data->size; 
        source->deleteFile(smallest->data->name);
    }
    if (folderSize == 0) return -1;
    return 0;
}

int main(int argc, char **argv) {
    int n, availableSize;  
    scanf("%d", &n);

    Folder *document = new Folder("document");
    Folder *another = new Folder("other");

    for (int i = 0; i < n; i++) {
        char filename[STR_LEN];
        int size; 
        std::cin.ignore();
        fgets(filename, sizeof(filename), stdin);
        int j = 0; 
        while (filename[j] != '\0') {
            j++;
        }  // Remove leading and trailing whitespace from the input string.
        filename[j - 1] = '\0';  // Remove newline character from the input string.

        scanf("%d", &size); 
        
        File *file = new File(filename, size);
        document->addFile(file);
    }
    scanf("%d", &availableSize);
    std::cout << "Size of document folder: " << document->getSizeFolder() << " MB" << std::endl;

    std::cout << "Files in document folder: " << std::endl;  // Print the files in the document folder.
    document->printFolder();

    document->deleteFile("Quoc");
    std::cout << "Files in document folder after deleted: " << std::endl;  // Print the files in the document folder.
    document->printFolder();


    fileCopy("Anh", document, another); 
    
    std::cout << "Other document after copy paste: " << std::endl;
    another->printFolder();

    std::cout << "After transfer to USB: " << std::endl;
    transferToUSB(document, availableSize);

    document->printFolder();
}
            this->size = size;  // Assign the size to the object.
        }
        ~Data() {}
};

class File {
    public: 
        File *nextFile; 
        Data *data;
        File (char *name, int size) {
            nextFile = NULL;
            data = new Data(name, size);  // Initialize data pointer to nullptr, indicating no data associated with the file yet.
        }
        ~File() {
            if(nextFile != NULL) delete nextFile;
            if(data != NULL) delete data;
        }
}; 

class Folder {
    public:
        File *firstFile;
        File *lastFile;
        char name[STR_LEN];

        Folder(char *name) {
            firstFile = NULL;
            lastFile = NULL;
            int i = 0;
            while (name[i] != '\0') {
                this->name[i] = name[i];
                i++;
            }
            this->name[i] = '\0';
        }

        ~Folder() {
            if(firstFile != NULL) delete firstFile; 
            if(lastFile != NULL) delete lastFile;
        }
        // Function Copy, Delete smallest file, Get size folder, Add files

        void addFile(File *file) {
            if (this->firstFile == NULL) {
                this->firstFile = file;
                this->lastFile = file;
            }
            else {
                this->lastFile->nextFile = file;
                this->lastFile = file;
            }
        }

        void printFolder() {
            File *current = this->firstFile;
            while (current != NULL) {
                std::cout << current->data->name << " (" << current->data->size << " MB)" << std::endl;
                current = current->nextFile;
            }
        }

        void deleteFile(char *name) {
            File *current = this->lastFile;
            File *prev = NULL; 

            if (this->firstFile != NULL and strcmp(this->firstFile->data->name, name) == 0) {
                if (this->firstFile == this->lastFile) this->lastFile = NULL;
                this->firstFile = this->firstFile->nextFile;
                delete current;
                return;
            }

            while (current!= NULL and strcmp(current->data->name, name) != 0) {
                prev = current;
                current = current->nextFile;
            }

            if (current == NULL) return;
            prev->nextFile = current->nextFile;
            if (current == this->lastFile) this->lastFile = prev;
            delete current;
            return;

        }
        // #define DEBUG
        int getSizeFolder() {
            if (this->firstFile == NULL) return 0; 
            File *current = this->firstFile;
            int size = 0; 
            while (current != NULL) {
                size += current->data->size;  // Add size of each file to total size.
                current = current->nextFile;
            }
            return size; 
        }

        File *findFile(char *name) {
            File *current = this->firstFile; 
            if (current == NULL) return NULL; 
            while (strcmp(current->data->name, name) != 0 and current != NULL) {

                #ifdef DEBUG
                std::cout << current->data->name << " " << strlen(current->data->name);
                {   
                    int j = 0;
                    while (current->data->name[j] != '\0') {
                        j++;
                    }  // Remove leading and trailing whitespace from the input string.
                    current->data->name[j - 1] = '\0';  // Remove newline character from the input string.
                    std::cout << j << std::endl;
                }
                
                #endif

                current = current->nextFile;
            }
            if (current == NULL) return NULL;
            return current;
        }

        File* getSmallestFile() {
            File *resFile = this->firstFile;
            File *current = this->firstFile;
            while (current != NULL) {
                if (current->data->size < resFile->data->size) {
                    resFile = current;
                }
                current = current->nextFile;
            }
            return resFile; 
        }
}; 

bool fileCopy(char *filename, Folder *source, Folder *destination) {
    File *file = source->findFile(filename);
    if (file == NULL) return false;
    File *newFile = new File(file->data->name, file->data->size);
    destination->addFile(newFile);
    return true;
}

int transferToUSB(Folder *source, int availableSize) {
    int folderSize = source->getSizeFolder(); 
    while (folderSize > availableSize) {
        File *smallest = source->getSmallestFile(); 
        if (smallest == NULL) return -1; 
        folderSize -= smallest->data->size; 
        source->deleteFile(smallest->data->name);
    }
    if (folderSize == 0) return -1;
    return 0;
}

int main(int argc, char **argv) {
    int n, availableSize;  
    scanf("%d", &n);

    Folder *document = new Folder("document");
    Folder *another = new Folder("other");

    for (int i = 0; i < n; i++) {
        char filename[STR_LEN];
        int size; 
        std::cin.ignore();
        fgets(filename, sizeof(filename), stdin);
        int j = 0; 
        while (filename[j] != '\0') {
            j++;
        }  // Remove leading and trailing whitespace from the input string.
        filename[j - 1] = '\0';  // Remove newline character from the input string.

        scanf("%d", &size); 
        
        File *file = new File(filename, size);
        document->addFile(file);
    }
    scanf("%d", &availableSize);
    std::cout << "Size of document folder: " << document->getSizeFolder() << " MB" << std::endl;

    std::cout << "Files in document folder: " << std::endl;  // Print the files in the document folder.
    document->printFolder();

    document->deleteFile("Quoc");
    std::cout << "Files in document folder after deleted: " << std::endl;  // Print the files in the document folder.
    document->printFolder();


    fileCopy("Anh", document, another); 
    
    std::cout << "Other document after copy paste: " << std::endl;
    another->printFolder();

    std::cout << "After transfer to USB: " << std::endl;
    transferToUSB(document, availableSize);

    document->printFolder();
}