
#include<iostream>
#include<fstream>
#include"helper.hpp"
#include<iomanip>


/**
 * A method can read the whole file and show how many command are there
 */
void readCommand (char *command_name){
    Command command;

    clear_cache();

    const int page_size = 20;
    std::ifstream fin(".command.bin",std::ios::in | std::ios::binary);
    
    if (!fin.is_open()){
        std::cerr<<"Something went wrong!\n";
        return;
    }


    int element_print = 0;
    while (fin.read((char*)&command,sizeof(command))){
        std::string  active = command.isactive ? "\u2714":"\u2716";
        std::cout <<active<<" [command name :"<<std::setw(30)<< command.commandname<<", Description : "<<command.description <<"]"<<std::endl;
        
        
        // future update
        if (++element_print == page_size){
            element_print = 0;
        }
    }


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
   
    std::ifstream fin (".command.bin",std::ios::in | std::ios::binary);

    if (find(fin,command_name)!=-1){

        std::cout<<"already \""<<command_name<<"\" rule is defined. choose another name"<<std::endl;
        fin.close();
        return; 
    }
  
    fin.close();


    clear_cache(true);
    
    std::fstream finout(".command.bin", std::ios::in | std::ios::out | std::ios::binary);
    
    if (!finout.is_open()){
        std::cerr<<"Something went wrong !";
        return;
    }

    strcpy(command.commandname,command_name);



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

    std::ifstream fin (".command.bin",std::ios::in | std::ios::binary );
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

    std::ifstream fin (".command.bin",std::ios::in|std::ios::binary);
    
    int pos=0;
    if ((pos = find(fin,commandname))!=-1){

        fin.seekg(pos,std::ios::beg);
        fin.read((char*)&command,sizeof(Command));

        std::cout <<"[command name:"<< command.commandname<<", Description :"<<command.description <<"]"<<std::endl;

    }else{
        std::cout <<"No Item exists of name :"<<commandname<<"\n";
    }
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

    std::ofstream fout (".command.bin",std::ios::out| std::ios::trunc |std::ios::binary);

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

    std::ifstream fin (".command.bin",std::ios::in | std::ios::binary);

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

    std::ofstream fout (".command.bin",std::ios::out|std::ios::in|std::ios::binary);
    fout.seekp(pos,std::ios::beg);

    fout.write((char*)&command,sizeof(Command));

    std::cout <<"Rule "<<commandname<<" is delete !"<<std::endl;

    fout.close();


    clear_cache();

}