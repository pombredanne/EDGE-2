#include <windows.h>
#include <imagehlp.h>
#include <string>
#include <vector>
#pragma comment(lib, "Dbghelp.lib")
#pragma comment(lib, "Imagehlp.lib")

void ListDLLFunctions(std::string sADllName, std::vector<std::string> &slListOfDllFunctions) {
    DWORD *dNameRVAs(0);
    _IMAGE_EXPORT_DIRECTORY *ImageExportDirectory;
    unsigned long cDirSize;
    _LOADED_IMAGE LoadedImage;
    std::string sName;
    slListOfDllFunctions.clear();
    if (MapAndLoad(sADllName.c_str(), NULL, &LoadedImage, TRUE, TRUE)) {
        ImageExportDirectory = (_IMAGE_EXPORT_DIRECTORY*)
            ImageDirectoryEntryToData(LoadedImage.MappedAddress,
            false, IMAGE_DIRECTORY_ENTRY_EXPORT, &cDirSize);
        if (ImageExportDirectory != NULL) {
            dNameRVAs = (DWORD *)ImageRvaToVa(LoadedImage.FileHeader,
                LoadedImage.MappedAddress,
            ImageExportDirectory->AddressOfNames, NULL);
            for(size_t i = 0; i < ImageExportDirectory->NumberOfNames; i++) {
                sName = (char *)ImageRvaToVa(LoadedImage.FileHeader,
                        LoadedImage.MappedAddress,
                        dNameRVAs[i], NULL);
                slListOfDllFunctions.push_back(sName);
            }
        }
        UnMapAndLoad(&LoadedImage);
    }
}

#ifdef LIST_BUILD_DEMO
#include <iostream>
int main(int argc, const char** argv) {
    std::vector<std::string> names;
    ListDLLFunctions( argc > 1 ? argv[1] : "KERNEL32.DLL", names);
    for( auto &it : names ) {
        std::cout << it << std::endl;
    }
}

#endif
