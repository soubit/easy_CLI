
#include<iostream>
#include<fstream>
#include"helper.hpp"
#include<iomanip>

#include"table_print.h"

/**
 * A method can read the whole file and show how many command are there
 */
extern std::string rulebook ;

void readCommand (char *command_name){
    Command command;

    clear_cache();

    const int page_size = 20;
    std::ifstream fin(rulebook.c_str(),std::ios::in | std::ios::binary);
    
    if (!fin.is_open()){
        std::cerr<<"Something went wrong!\n";
        return;
    }
    set_column_no(3);

    set_header("Active",LEFT,9);
    set_header("Command-Name",LEFT,30);
    set_header("Description",LEFT,40);

    print_header();
    // print all command serial by serial
    int element_print = 0;
    while (fin.read((char*)&command,sizeof(command))){
        // std::string  active = command.isactive ? "\u2714":"\u2716";
        std::string  active = command.isactive ? "val":"del";
        // printf ("%d",strlen(active.c_str()));
        // const char *ptr = active.c_str();
        // for (int i = 0 ; i<strlen(ptr);i++){
        //     printf("%x-",ptr[i]);
        // }
        print_row(active.c_str(),command.commandname,command.description);
        
        
        // future update
        if (++element_print == page_size){
            element_print = 0;
        }
    }
    // print_row("Hello","Sovuk","dd");
    // print_row("Hello","Sovuk","hihiuhiuhgujgjygj mgj iuhiuhikh huhihniuh uhhiuhih klikuhb");
    
    clear_table();
    std::cout<<std::endl;



    std::cout <<std::left<<std::setw(23)<<"File size "<<bookSize()<<" Bytes."<<std::endl;
    std::cout <<std::left<<std::setw(23)<<"each entry size :"<<sizeof(Command)<<" Bytes."<<std::endl;
    std::cout <<std::left<<std::setw(23)<<"Total number of rule :"<<bookSize() / sizeof(Command)<<"."<<std::endl;

    fin.close();
}



/**
 * A method can help to write a new command for the easyopen
 */
void writeNewCommand(char *template_name,char *command_name=NULL){

    bool template_on = true;

    if (command_name==NULL){
        command_name = template_name;
        template_on = false;
    }

    Command command;
    // memset(&command,0,sizeof (command));
   
    std::ifstream fin (rulebook.c_str(),std::ios::in | std::ios::binary);
    
    if (!fin){
        std::cerr<<"RuleBook is not found!\n";
        exit(-1);
    }

    // search if there already a command with same name
    if (find(fin,command_name)!=-1){

        std::cout<<"already \""<<command_name<<"\" rule is defined. choose another name"<<std::endl;
        fin.close();
        return; 
    }
  
    fin.close();


    clear_cache(true);
    // write a new command


    std::fstream finout(rulebook.c_str(), std::ios::in | std::ios::out | std::ios::binary);
    
    
    if (!finout.is_open()){
        std::cerr<<"Something went wrong !";
        return;
    }

    strcpy(command.commandname,command_name);


    // if template command is come then write command by the help of template engine
    if (template_on){
        std::string execommand = template_engine(template_name);
        if (execommand.length()>0){
            
            strcpy(command.execommand,execommand.c_str());
        }else{
            std::cerr<<"Its not a valid command"<<std::endl;
            return ;
        }
    }else{
        std::cout<<"what will be the command for the "<<command_name<<":";
        std::cin.getline(command.execommand,(sizeof(command.execommand)/sizeof(char)));
    }


    std::cout <<"Description :";
    std::cin.getline(command.description,(sizeof(command.description)/sizeof(char)));

    command.isactive = true;
 
    isort(finout,command);

    // code for sort and input data
    // read and compare  where should we entry the data 
    // then left down all entry

    
    finout.close();
    std::cout <<"New Entry done for rule :"<<command_name<<"\n";
}




/**
 * Method can help the system to execute the predefined code
 *  */
void executeCommand(char *command_name){
    
    Command command;

    std::ifstream fin (rulebook.c_str(),std::ios::in | std::ios::binary );
    if (!fin.is_open()){
        std::cerr<<"something went wrong!";
        return;
    }

    int pos = find(fin,command_name);
    if(pos!=-1){
        fin.seekg(pos,std::ios::beg);

        fin.read((char*)&command,sizeof(Command));

        system(command.execommand);
        fin.close();

        // clear_cache();  // clear un-necessary entry;
        clear_cache();
        
        return;
    }

    
    std::cout<<"No Command found of this name \n";
    fin.close();

}   

/**
 * Search if there already a the rule is present or not 
 */
void searchName(char * commandname){
    
    Command command;

    std::ifstream fin (rulebook.c_str(),std::ios::in|std::ios::binary);
    
    set_column_no(3);
    set_header("Command-Name",LEFT,20);
    set_header("Command ",LEFT,65);
    set_header("Description",LEFT,70);

    // printf ("search is called");

    print_header();

    
    int pos=0;
    if ((pos = find(fin,commandname))!=-1){

        fin.seekg(pos,std::ios::beg);
        fin.read((char*)&command,sizeof(Command));

        // std::cout <<"[command name:"<< command.commandname<<", Description :"<<command.description <<"]"<<std::endl;
        print_row(command.commandname,command.execommand,command.description);

    }else{
        std::cout <<"No Item exists of name :"<<commandname<<"\n";
    }

    clear_table();
    fin.close();

    clear_cache();
}



/**
 * trunc the rule-book
 */
void clearFile (){
    std::cout <<"Do you want to clear all commands ?(press y for yes):";
    if (std::getc(stdin)!='y'){
        return;
    }

    std::ofstream fout (rulebook.c_str(),std::ios::out| std::ios::trunc |std::ios::binary);

    std::cout <<"All rules are clear\n";

    if (!fout.is_open()){
        std::cerr<<"Something went wrong !\n";
    }
}


/**
 * Delete any entry from the rule book;
 */
void deleteEntry(char *commandname){
    Command command;

    std::ifstream fin (rulebook.c_str(),std::ios::in | std::ios::binary);

    if (!fin){
        std::cerr<<"Something went problem!"<<std::endl;
        return;
    }


    int pos = find(fin,commandname);    // if its already deleted or not found

    if (pos==-1){
        std::cerr<<"Entered Entry is not found!"<<std::endl;
        return;
    }

    fin.seekg(pos,std::ios::beg);

    fin.read((char*)&command,sizeof(Command));
    
    command.isactive = false;

    fin.close();

    std::ofstream fout (rulebook.c_str(),std::ios::out|std::ios::in|std::ios::binary);
    fout.seekp(pos,std::ios::beg);

    fout.write((char*)&command,sizeof(Command));

    std::cout <<"Rule "<<commandname<<" is delete !"<<std::endl;

    fout.close();


    clear_cache();

}