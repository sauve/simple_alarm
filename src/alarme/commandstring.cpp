#include "commandstring.h"


void CommandString::clear()
{
  cmdlen = 0;
  nbrarg = 0;
  for (int i = 0; i < 64; i++)
  {
    buffer[i] = 0;
  }
  index = 0;
  for (int i = 0; i < 4; i++)
  {
    argstart[i] = 0;
    arglen[i] = 0;
  }
}

void CommandString::addChar( char c)
{
  // index 63 doit etre un 0 pour la fin de la chaine
  if ( index < 62 )
  {
    buffer[index] = c;
    index++;

    // si c'est une virgule, ajoute un argument, sinon augmente la taille de nom de la commande ou de l'argument courant
    if (c == ',')
    {
      buffer[index-1] = '\0';
      argstart[nbrarg] = index;
      nbrarg++;
    }
    else
    {
      if (nbrarg > 0)
      {
        arglen[nbrarg-1] = index - argstart[nbrarg-1];
      }
      else
      {
        cmdlen = index;
      }
    }
  }
}

void CommandString::debugPrint()
{
  Serial.print("cmd: ");
  for (int i = 0; i < cmdlen; i++)
  {
    Serial.print(buffer[i]);
  }
  Serial.println();
  for (int i = 0; i < nbrarg; i++)
  {
    Serial.print("arg");
    Serial.print(i);
    Serial.print(": ");
    for (int j = 0; j < arglen[i]; j++)
    {
      Serial.print(buffer[argstart[i]+j]);
    }
    Serial.println();
  }
}