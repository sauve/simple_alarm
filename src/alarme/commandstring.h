
#include <Arduino.h>
#include <avr/pgmspace.h>

class CommandString
{
public:
  void clear();
  void addChar(char c);

  // assignation a partir d'une chaine

  // retourne le nom de la commande
  // compare si c'est la commande
  int cmpCmd( const char* cmd)
  {
    return strncmp(buffer, cmd, cmdlen);
  }

  // compare le nom de la commande avec un string en PROGMEM
  int cmpCmd_P( const char* cmd)
  {
    return strcmp_P(buffer, cmd);
  }
  // retourne le nombre d'argument
  int nbrArg()
  {
    return nbrarg;
  }

  // compare l'argument a l'index
  int cmpArg( int index, const char* arg)
  {
    if (index >= nbrarg) return 1;
    return strncmp(buffer + argstart[index], arg, arglen[index]);
  }

  // retourne l'argument en tant que int
  int getArgInt( int index)
  {
    if (index >= nbrarg) return 0;
    return atoi(buffer + argstart[index]);
  }

  void debugPrint();

protected:
  char buffer[64];
  int index;
  int cmdlen;
  int nbrarg;
  int argstart[4];
  int arglen[4];
};