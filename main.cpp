/*
 *
 * Copyright (c) 2022 TheMrCerebro
 *
 * PAKex - Zlib license.
 *
 * This software is provided 'as-is', without any express or
 * implied warranty. In no event will the authors be held
 * liable for any damages arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute
 * it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented;
 * you must not claim that you wrote the original software.
 * If you use this software in a product, an acknowledgment
 * in the product documentation would be appreciated but
 * is not required.
 *
 * 2. Altered source versions must be plainly marked as such,
 * and must not be misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any
 * source distribution.
 *
*/

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <direct.h>

using namespace std;

struct SPAKFileHeader
{
    char tag[4];
    unsigned int offset;
    unsigned int length;
};

struct SPAKFileEntry
{
    char name[56];
    unsigned int offset;
    unsigned int length;
};

int main()
{
    cout << "               _______  _______  ___   _  _______  __   __ " << endl;
    cout << "              |       ||   _   ||   | | ||       ||  |_|  |" << endl;
    cout << "              |    _  ||  |_|  ||   |_| ||    ___||       |" << endl;
    cout << "              |   |_| ||       ||      _||   |___ |       |" << endl;
    cout << "              |    ___||       ||     |_ |    ___| |     | " << endl;
    cout << "              |   |    |   _   ||    _  ||   |___ |   _   |" << endl;
    cout << "              |___|    |__| |__||___| |_||_______||__| |__|" << endl;
    cout << " (o)=================================================================(o)" << endl;
    cout << "          - FAST AND EASY PAK FILE EXTRACTOR [by TheMrCerebro] -" << endl;
    cout << " (o)=================================================================(o)" << endl;
    cout << endl;
    cout << "     With this tool you can extract all files contained in any PAK" << endl;
    cout << "              file used by Half-life and Quake games." << endl;
    cout << endl;

    cout << " File: ";

    string path;
    getline(cin,path);

    cout << endl;

    // Crea la carpeta principal con el nombre del archivo ".pak" donde se va a extraer el contenido.
    std::string folder;
    const char* s = path.c_str();
    const char* p = s + path.size();

    while( *p != '/' && *p != '\\' && p != s )
        p--;

    if( p != s )
    {
        ++p;
        folder = p;
    }

    // Elimina la extension del nombre
	size_t endPos = folder.find(".");
	folder = folder.substr( 0, endPos < 0 ? folder.size () : endPos );

    _mkdir(folder.c_str());
    _chdir(folder.c_str());

    // Hubicacion de la carpeta principal.
    char cwd[256];
    _getcwd(cwd, 256);

	SPAKFileHeader header;

    ifstream pf(path.c_str(), ios::binary);
    pf.read(reinterpret_cast<char*>(&header), sizeof(header));

    char* tag = header.tag;
	if(tag[0] == 'P' && tag[1] == 'A' && tag[2] == 'C' && tag[3] == 'K')
    {
        cout << " Valid PAK file" << endl;
        cout << endl;
        cout << " Extracting..." << endl;
        cout << endl;

        pf.seekg(header.offset, ios::beg);

        const int numberOfFiles = header.length / sizeof(SPAKFileEntry);

        vector<SPAKFileEntry> entryA;
        for(int i=0; i<numberOfFiles; i++)
        {
            // read an entry
            SPAKFileEntry entry;
            pf.read(reinterpret_cast<char*>(&entry), sizeof(entry));
            entryA.push_back(entry);
        }

        for(int i=0; i<numberOfFiles; ++i)
        {
            string tmp;
            for(size_t j=0; j<strlen(entryA[i].name); ++j)
            {
                if(entryA[i].name[j]=='/')
                {
                    if((int)tmp.find(".") > -1) break;
                    _mkdir(tmp.c_str());
                    _chdir(tmp.c_str());
                    tmp.clear();
                }
                else
                {
                    tmp += entryA[i].name[j];
                }
            }

            _chdir(cwd);

            char *buffer = new char[entryA[i].length];

            pf.seekg(entryA[i].offset, ios::beg);
            pf.read(buffer, entryA[i].length);

            ofstream wf(entryA[i].name, ios::binary);
            wf.write(buffer, entryA[i].length);
            wf.close();

            delete [] buffer;

            cout << "  " << folder << "/" << entryA[i].name << endl;
        }

        cout << endl;
        cout << " ...Finished" << endl;
        cout << endl;
        cout << " Total Files: " << numberOfFiles << endl;
        cout << endl;
    }
    else
    {
        cout << " Invalid PAK file" << endl;
        cout << endl;
    }

    pf.close();

    system("pause");
    return 0;
}
