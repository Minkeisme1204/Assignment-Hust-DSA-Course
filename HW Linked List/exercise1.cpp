#include <iostream>
#include <chrono>

#define STR_LEN 50
#define FLAG_TIME 0
#define FLAG_SIZE 1

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
        int timeCreate; // Creation time

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

        Folder() {}

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
                std::cout << current->data->name << " (" << current->data->size << " MB) " << "time created: " << current->data->timeCreate << std::endl;
                // std::cout << &current << std::endl; 
                current = current->nextFile;
            }
        }

        void deleteFile(char *name) {
            if (this->firstFile == NULL) return;  

            if (strcmp(this->firstFile->data->name, name) == 0) {
                File* temp = this->firstFile;
                this->firstFile = this->firstFile->nextFile;
                delete temp;
                return;
            }

            File* current = this->firstFile;
            File* prev = NULL;
            while (current != NULL && strcmp(this->firstFile->data->name, name) != 0) {
                prev = current;
                current = current->nextFile;
            }

            if (current == NULL) return;

            prev->nextFile = current->nextFile;
            delete current;
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

        File* getLastFile() {
            File *current = this->firstFile; 
            while (current->nextFile != NULL) {
                current = current->nextFile; 
            }
            return current; 
        }

        void sortFilesInFolder(int flag) {
            if(flag == FLAG_SIZE) { // bubble sort 
                File *current = this->firstFile;
                while (current != NULL) {
                    File *i = current; 
                    while (i != NULL) {
                        if (i->data->size == current->data->size) {
                            if (i->data->timeCreate < current->data->timeCreate) {
                                Data *temp = i->data;
                                i->data = current->data;
                                current->data = temp;
                                continue; 
                            }
                        }
                        else if (i->data->size < current->data->size) {
                            Data *temp = i->data;
                            i->data = current->data;
                            current->data = temp;
                            continue; 
                        }
                        i = i->nextFile;
                    }
                    current = current->nextFile;
                }
            }
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
    auto start = std::chrono::high_resolution_clock::now();
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
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds> (end - start);
        file->data->timeCreate = (int)duration.count()/1000;
        document->addFile(file);
        
    }
    document->sortFilesInFolder(FLAG_SIZE);

    std::cout << "Folder newly created: " << std::endl;
    document->printFolder();

    std::cout << "Folder after deleting the smallest file: " << std::endl;
    document->deleteFile(document->firstFile->data->name);
    document->printFolder();
}