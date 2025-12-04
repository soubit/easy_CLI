#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>

#include "./helper.hpp"
#include "./Fileop.hpp"

Command command;




// usage : easyopen [option] command_name
// option :
//      -r : find a command from the file
//      -w : write new command
//      -e : execute a existing command

int main (int argc , char * argv[]){
    
    if (argc > 4 || argc <=1){
        std::cerr<<"usage : codeopen [option] command_name\n for help : ./easyopen -h\n";
        return 1;
    }
    
    fileUpdate();   // update the file structure for new version

    if(argv[1][0]=='-'){
        switch(argv[1][1]){

            case 'r':
                readCommand(argv[2]);
                break;

            case 'w':
                if (argc == 3){
                    writeNewCommand (argv[2]);
                }else if (argc == 4){
                    // template store
                    writeNewCommand(argv[2],argv[3]);
                }
                else{
                    std::cerr <<"It must contain the new rule name\n";
                }
                break;

            case 'e':
                if (argc ==3){
                    executeCommand (argv[2]);
                }else{
                    system("./easyopen -h");
                }
                break;


            case 'h':
                std::cout <<"(C) SOUviK SaSmaL\n-h : help\n-r : read \n-w : write\n-e : execute\n-p : purge\n-s : search any item\n-d : delete any entry\n";
                break;


            case 'p':
                clearFile ();
                break;
            
            case 's':
                searchName (argv[2]);
                break;

            case 'd':
                deleteEntry(argv[2]);
                break;
                
            default :
                std::cerr <<"Not a valid option .\n";
                return 1;
        }
    }else{
        executeCommand (argv[1]);
    }
}

