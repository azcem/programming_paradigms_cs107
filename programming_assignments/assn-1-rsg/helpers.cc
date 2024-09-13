#include <map>
#include "definition.h"
#include "production.h"
using namespace std;

//takes a ruleName string and grammar and returns a vector of its expansion
vector<string> expandRule(map<string, Definition>* grammar, string rule) {
  Production root = (*grammar)[rule].getRandomProduction();
  vector<string> res;

  //populate
  res.insert(res.begin(), root.begin(), root.end());

  for (vector<string>::iterator it = res.begin(); it != res.end(); it ++) {
    if ((*it)[0] == '<') {
      vector<string> expansion = expandRule(grammar, *it);
      it = res.erase(it);
      it = res.insert(it, expansion.begin(), expansion.end());
    }
  }
  return res;
}

//print the vector of terminal words
//case for period add newline
//case for non comma add space
//case for comma dont add space
string printWords(vector<string> words) {
  string res = "";
  bool newLine = false;
  for (auto it = words.begin(); it != words.end(); it++) {
    if (it == words.begin()){ res = *it; continue;} 
    if (*it == ",") res += ",";
    else if (*it == ".") res += ".";
    else {
      if (newLine) res += *it;
      else res += ' ' + *it;
      newLine = false;
    }
    if (*((*it).rbegin()) == '.'){
      res += '\n';
      newLine = true;
    } 
  }
  return res;
}
