f document folder: " << document->getSizeFolder() << " MB" << std::endl;

    std::cout << "Files in document folder: " << std::endl;  // Print the files in the document folder.
    // std::cout << document->firstFile << " " << document->findFile("Nguyen") << std::endl; 

    fileCopy("Minh", document, another); 
    
    std::cout << "Other document after copy paste: " << std::endl;
    another->printFolder();

    std::cout << "After transfer to USB: " << std::endl;
    transferToUSB(document, availableSize);

    document->printFolder();