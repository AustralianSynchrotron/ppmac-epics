/*
 * pmacCommandStore.cpp
 *
 *  Created on: 11 Feb 2016
 *      Author: gnx91527
 */

#include "pmacCommandStore.h"
#include <stdio.h>
#include <string.h>
#include <algorithm>

#define PMAC_MAX_REQUESTS 40

pmacCommandStore::pmacCommandStore() :
        pmacDebugger("pmacCommandStore"),
        qtyCmdStrings(0) {
  int index = 0;
  for (index = 0; index < 100; index++) {
    strcpy(commandString[index], "");
  }
}

pmacCommandStore::~pmacCommandStore() {

}

int pmacCommandStore::addItem(const std::string &key) {
  this->store.insert(key, "");
  this->buildCommandString();
  return 0;
}

int pmacCommandStore::deleteItem(const std::string &key) {
  this->store.remove(key);
  this->buildCommandString();
  return 0;
}

bool pmacCommandStore::checkForItem(const std::string &key) {
  return this->store.hasKey(key);
}

std::string pmacCommandStore::readValue(const std::string &key) {
  return this->store.lookup(key);
}

int pmacCommandStore::size() {
  return this->store.count();
}

std::string pmacCommandStore::readCommandString(int index) {
  static const char *functionName = "readCommandString";
  std::string returnString = "";
  returnString.assign(this->commandString[index]);
  if (returnString.find_first_not_of(" ") != std::string::npos) {
    returnString = returnString.substr(returnString.find_first_not_of(" "));
  }
  debug(DEBUG_VARIABLE, functionName, "Command string", returnString);
  return returnString;
}

int pmacCommandStore::countCommandStrings() {
  return qtyCmdStrings;
}

int pmacCommandStore::updateReply(const std::string &cmd, const std::string &reply) {
  static const char *functionName = "updateReply";

  std::string keys = cmd;
  std::string data = reply;

  int status = 0;
  int running = 1;

  /* [MSP-226][MSP-261] 

  check if number of returned values match requested keys,
  drop the response if the numbers don't match, because it would be ambiguious to
  assiociate the values to keys.

  It is assumed that all of the key strings in this update sentences, 
  are expected to have one and only one response value
  
  */

  int numCmdTerms = std::count(keys.begin(), keys.end(), ' ') + 1;
  int numRspReturns = std::count(data.begin(), data.end(), '\r');
  int numCmdStats = std::count(keys.begin(), keys.end(), '=');
  int expectedReturns = numCmdTerms - numCmdStats;
  
  if ( (data.find("error") != std::string::npos) || ( expectedReturns != numRspReturns )){
    // For ppmac, this is an indication of either error or wrong variable names
    char errmsg[4196] = {0};
    sprintf(errmsg, "cmds(%i)->%s\n\nexpected %i, returned %i\n", numCmdTerms, keys.c_str(), expectedReturns, numRspReturns)   ; 
    debug(DEBUG_ERROR, functionName, "ERROR: mistmatch response\n", errmsg);
    // DO NOT process the response because there is no rleiable way to parse it!
    running = 0;
    status = 0;

    /* TODO; can we count up the frequency of this error and dump the IOC if the frequency is 
    high enough to conclude that the IOC parameters are incompatible with pmac?
    */

    /* Terminate here if this persists
    if (numRspReturns > 0){
      exit(1);
    }
    /*-----*/

  }
  /* ---------------- */

  while (data.find("\r") != std::string::npos && running == 1) {
    std::string val = data.substr(0, data.find("\r"));
    data = data.substr(data.find("\r") + 1);
    if (keys.find(" ") != std::string::npos) {
      std::string key = keys.substr(0, keys.find(" "));
      debug(DEBUG_VARIABLE, functionName, "KEY  ", key);
      debug(DEBUG_VARIABLE, functionName, "VALUE", val);
      keys = keys.substr(keys.find(" ") + 1);
      if (this->store.hasKey(key)) {
        this->store.insert(key, val);
      }
    } else {
      std::string key = keys;
      debug(DEBUG_VARIABLE, functionName, "KEY  ", key);
      debug(DEBUG_VARIABLE, functionName, "VALUE", val);
      if (this->store.hasKey(key)) {
        this->store.insert(key, val);
      }
      running = 0;
    }
  }

  /*
  std::string data = reply;
  // We need to loop over the reply string splitting each data item by \r
  std::string key = this->store.firstKey();
  if (key == ""){
    // No data items in the store
    running = 0;
  }
  while (data.find("\r") != std::string::npos && running == 1){
    std::string val = data.substr(0, data.find("\r"));
    data = data.substr(data.find("\r")+1);
//    printf("Key: %s   value: %s\n", key.c_str(), val.c_str());
    this->store.insert(key, val);
    if (this->store.hasNextKey()){
      key = this->store.nextKey();
    } else {
      running = 0;
    }
  }
  if (data.find("\r") != std::string::npos){
    // Too many data items
    debug(DEBUG_ERROR, functionName, "Too many data items supplied");
    status = -1;
  } else {
    if (running == 1){
      // Not enough data items
      debug(DEBUG_ERROR, functionName, "Not enough data items supplied");
      status = -1;
    }

  }
  this->unlock();
  */
  return status;
}

void pmacCommandStore::report() {
  std::string key = this->store.firstKey();
  printf("[%s] => %s\n", key.c_str(), this->store.lookup(key).c_str());
  while (this->store.hasNextKey()) {
    key = this->store.nextKey();
    printf("[%s] => %s\n", key.c_str(), this->store.lookup(key).c_str());
  }
}

void pmacCommandStore::buildCommandString() {
  int index = 0;
  qtyCmdStrings = 0;
  // Fill up command string buffers, MAX_VALS in each
  std::string key = this->store.firstKey();
  strcpy(commandString[qtyCmdStrings], "");
  sprintf(commandString[qtyCmdStrings], "%s %s", commandString[qtyCmdStrings], key.c_str());
  index++;
  while (this->store.hasNextKey()) {
    key = this->store.nextKey();
    sprintf(commandString[qtyCmdStrings], "%s %s", commandString[qtyCmdStrings], key.c_str());
    index++;
    if (index == PMAC_MAX_REQUESTS) {
      // Move onto next buffer
      index = 0;
      qtyCmdStrings++;
      strcpy(commandString[qtyCmdStrings], "");
    }
  }
  // Finally +1 to command strings
  qtyCmdStrings++;
  //printf("%s\n", commandString);
}


