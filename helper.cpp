#include"./helper.hpp"
#include<iostream>
#include<fstream>
#include<cstring>
#include<queue>
#include<cmath>
#include<cstdlib>
#include<stdbool.h>


std::string rulebook = std::string (FILELOC) + "/.command.bin";
std::string tempfile = std::string (FILELOC) + "/.temp";


namespace constant{
    bool is_smaller(int val){
        if (val<0)
            return true;
        else
            return false;
    }

    bool is_equal(int val){
        if (val == 0)
            return true;
        else   
            return false;
    }

    bool is_big(int val){
        if (val>0)
            return true;
        else    
            return false;

    }
}



int compare (Command c1 , Command c2){
    return strcmp(c1.commandname,c2.commandname);
}


// optimized sorting code O(n)
void isort (std::fstream &finout , const Command &command){
    // f-mode must :std::ios::in | std::ios::out| std::ios::binary

    Command temp ;
    std::queue<Command> q;
    int sizeentry = sizeof (Command);



    finout.seekg(0,std::ios::end);
    if(finout.tellg()==0){
        // file is empty
        finout.seekp(0,std::ios::end);
        finout.write((char*)&command,sizeentry);
        finout.clear();
        return;
    }

    int entrypos = 0;

    finout.seekg(0,std::ios::beg);
    // find point where should insert the new entry
    while (finout.read((char*)&temp, sizeentry)){
        if (constant::is_smaller(compare(command,temp))){
            break;
        }
        entrypos+=sizeentry;
    }
    finout.clear();

 
    // push the new one    
     q.push(command);

     finout.seekg(entrypos,std::ios::beg);
     while (finout.read((char*)&temp,sizeentry)){
         q.push(temp);
     }

     // write all command
     finout.clear();
     finout.seekp(entrypos,std::ios::beg);   
     while (!q.empty()){
         temp = q.front();
         q.pop();
         finout.write((char*)&temp,sizeentry);
     }

}



/**
 * find the entry and return the position
 */

int find (std::ifstream &fin , const char* commandnaem){
    // f-open mode - std::ios::in | std::ios::binary

    Command temp;
    int sizeentry = sizeof (Command);


    fin.seekg(0,std::ios::end);

    int left_ptr = 0 ;
    int right_ptr = fin.tellg() != 0 ? (int(fin.tellg())/sizeentry)-1 : 0;
    
    fin.clear();


    while (left_ptr <= right_ptr){
        int mid = (left_ptr + right_ptr)/2;

        fin.seekg(mid*sizeentry,std::ios::beg);

        if(!fin.read((char*)&temp,sizeentry)){
            break;
        }
        fin.clear();

        if (constant::is_equal(strcmp(commandnaem,temp.commandname))){
            if (temp.isactive)
                return mid*sizeentry;    // item found
            else    
                return -1;   // entry is deleted
        }else if (constant::is_big(strcmp(commandnaem,temp.commandname))){
            left_ptr = mid+1;
        }else{
            right_ptr = mid-1;
        }
    }

    return -1;

}


/**
 * Delete unwanted entry (deleted entry )
 */
void flush_bin(){
    std::ifstream fin (rulebook.c_str(),std::ios::in|std::ios::binary);
    std::ofstream fout (tempfile.c_str(),std::ios::out|std::ios::binary);

    Command command;

    if (!fin || !fout){
        std::cerr<<"something went problem to clear cache!";
        return;
    }


    while (fin.read((char*)&command,sizeof(Command))){
        if (command.isactive){  // those file are active will push into active file (put back)
            fout.write((char*)&command,sizeof(Command));
        }
    }

    fin.close();
    fout.close();


    remove(rulebook.c_str());
    rename(tempfile.c_str(),rulebook.c_str());
    
}


/**
 * delete cache random time
 */
void clear_cache (bool now){
    if (now){
        flush_bin();
    }else{
        srand(time(NULL));

        int random_number = (rand() % 10);

        if (random_number % 2){
            // when odd
            flush_bin();
        }
    }
}       



size_t bookSize(){
    std::ifstream fin (rulebook.c_str(),std::ios::in | std::ios::binary);
    if (!fin){
        std::cerr<<"something went wrong!"<<std::endl;
        return -1;
    }

    fin.seekg(0,std::ios::end);
    int storage = fin.tellg();
    fin.close();
    return storage;
}

void fileUpdate(){
    // algo 
    /**
     * first check the size of structure 
     * if mismatch then update the file system  
     */
    int size_entry = sizeof (Command);

    if (bookSize() % size_entry != 0 ){
        // file is not update as the new rule structure

        std::ifstream fin (rulebook,std::ios::in|std::ios::binary);

        if (!fin){
            std::cerr<<"Rulebook is not Found for update file"<<std::endl;
            return;
        }


        std::ofstream fout (tempfile.c_str(),std::ios::out | std::ios::binary);

        if (fout.is_open()){
           
    
            OldCommand oldcommand;
            Command command;

            while (fin.read((char*)&oldcommand,sizeof(oldcommand))){
                command.copyData(oldcommand);
                // std::cout <<command;
                fout.write((char*)&command,sizeof(command));
            }


            fin.close();
            fout.close();


            remove(rulebook.c_str());
            rename(tempfile.c_str(),rulebook.c_str());


            std::cout<<"File system is upgrade!"<<std::endl;
        }



        
    }


}



// helper function for the template_engine() function 
int template_no (char* name){
    if (strcmp(name,"--google")==0){
        return 1;
    }else{
        return 0;
    }
}



 std::string template_engine (char *template_name){
    int option = template_no(template_name);

    switch(option){
        case 1: {
            //google
            // this version is default for profile-2 in my system
            std::string template_ = "google-chrome --profile-directory=\"Profile 2\" ";
            const int website_name_size = 200 - template_.length() + 15;
            
            std::cout <<"Enter the website link :";
            std::string  website_name(website_name_size,'\0');

            std::getline(std::cin,website_name);
            return (template_ + "\"" +website_name + "\" &");
        }

        default:
            // std::cerr<<"Not a valid template !"<<std::endl;
            return std::string();
    }
 }