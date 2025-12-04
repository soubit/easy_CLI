#ifndef HELPER_H
#define HELPER_H


#include <fstream>
#include <cstring>


// namespace constant{
//     bool is_smaller(int val);

//     bool is_equal(int val);

//     bool is_big(int val);
// }



struct OldCommand{
    char commandname[50];
    char execommand[100];
    char description[200];
    bool isactive;

    void operator= (OldCommand &c){
        strcpy(commandname,c.commandname);
        strcpy(execommand,c.execommand);
        strcpy(description,c.description);
    }
    
};


struct  Command{
    
    char commandname[30];
    char execommand[200];
    char description[200];
    bool isactive;
    

    void operator= (Command &c){
        strcpy(commandname,c.commandname);
        strcpy(execommand,c.execommand);
        strcpy(description,c.description);
    }
    

    void copyData (OldCommand &oldcommand){
        strcpy(commandname,oldcommand.commandname);
        strcpy(execommand,oldcommand.execommand);
        strcpy(description,oldcommand.description);

        isactive = oldcommand.isactive;
    }

    friend std::ostream& operator<< (std::ostream &cout,Command command){
        cout <<"["<<command.commandname<<":"<<command.execommand<<":"<<command.description<<":"<<command.isactive<<"]"<<"\n";
        return cout;
    }
};





/**
 * a helper method that can help to sort a entry while inserting 
 */
void isort (std::fstream &finout, const Command &command);




/**|
 *  help to find if any entry exist for the command name
 */
int find (std::ifstream &fin , const char *commandname);


/***
 * remove all delted entry 
 */
void flush_bin();


/**
 * flush cache in random time
 */
void clear_cache(bool=false);



/**
 * when the software is update if the structure is update of the file then the file data will automaticaly update
 */
void fileUpdate();



/**
 * size of the rule book
 */

 size_t bookSize();




 /**
  * template string or command name for the rule book
  */

  std::string  template_engine (char*);
#endif 