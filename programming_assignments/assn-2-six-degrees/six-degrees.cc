#include <vector>
#include <map>
#include <list>
#include <set>
#include <strings.h>
#include <iostream>
#include <iomanip>
#include "imdb.h"
#include "path.h"
using namespace std;

/**
 * Using the specified prompt, requests that the user supply
 * the name of an actor or actress.  The code returns
 * once the user has supplied a name for which some record within
 * the referenced imdb existsif (or if the user just hits return,
 * which is a signal that the empty string should just be returned.)
 *
 * @param prompt the text that should be used for the meaningful
 *               part of the user prompt.
 * @param db a reference to the imdb which can be used to confirm
 *           that a user's response is a legitimate one.
 * @return the name of the user-supplied actor or actress, or the
 *         empty string.
 */

static string promptForActor(const string& prompt, const imdb& db)
{
  string response;
  while (true) {
    cout << prompt << " [or <enter> to quit]: ";
    getline(cin, response);
    if (response == "") return "";
    vector<film> credits;
    if (db.getCredits(response, credits)) return response;
    cout << "We couldn't find \"" << response << "\" in the movie database. "
	 << "Please try again." << endl;
 }
}

void generateShortestPath(imdb& db, const string& start, const string& target) {
  list<path> partialPaths;
  set<string> previouslySeenActors;
  set<film> previouslySeenFilms;

  map<string, vector<film>> actorFilmCache;
  map<film, vector<string>> filmCastCache;

  path startPath = path(start);
  partialPaths.push_back(startPath);
  while (partialPaths.size() > 0 && partialPaths.front().getLength() < 6) {
    path workingPath = partialPaths.front();
    partialPaths.pop_front();
    string actor = workingPath.getLastPlayer();
    vector<film> films;
    auto cached_films = actorFilmCache.find(actor);
    if (cached_films == actorFilmCache.end()) {
      db.getCredits(actor, films);
      actorFilmCache.insert({actor, films});
    }
    else {
      films = cached_films->second;
    }
    for (auto film : films) {
      if (!previouslySeenFilms.insert(film).second) continue;
      vector<string> cast;
      auto cached_cast = filmCastCache.find(film);
      if (cached_cast == filmCastCache.end()){
        db.getCast(film, cast);
        filmCastCache.insert({film, cast});
      }
      else{
        cast = cached_cast->second;
      }
      for (auto member : cast) {
        if (!previouslySeenActors.insert(member).second) continue;
        path clonedPath = path(workingPath);
        clonedPath.addConnection(film, member);
        if (member == target) {
          cout << clonedPath;
          return;
        } else {
          partialPaths.push_back(clonedPath);
        }
      }
    }
  }
  cout << "Couldn't find a path :(" << endl;
}

/**
 * Serves as the main entry point for the six-degrees executable.
 * There are no parameters to speak of.
 *
 * @param argc the number of tokens passed to the command line to
 *             invoke this executable.  It's completely ignored
 *             here, because we don't expect any arguments.
 * @param argv the C strings making up the full command line.
 *             We expect argv[0] to be logically equivalent to
 *             "six-degrees" (or whatever absolute path was used to
 *             invoke the program), but otherwise these are ignored
 *             as well.
 * @return 0 if the program ends normally, and undefined otherwise.
 */

int main(int argc, const char *argv[])
{
  // imdb db(determinePathToData(argv[1])); // inlined in imdb-utils.h
  imdb db("../assn-2-six-degrees-data/little-endian/"); // inlined in imdb-utils.h
  if (!db.good()) {
    cout << "Failed to properly initialize the imdb database." << endl;
    cout << "Please check to make sure the source files exist and that you have permission to read them." << endl;
    exit(1);
  }
  
  while (true) {
    string source = promptForActor("Actor or actress", db);
    if (source == "") break;
    string target = promptForActor("Another actor or actress", db);
    if (target == "") break;
    if (source == target) {
      cout << "Good one.  This is only interesting if you specify two different people." << endl;
    } else {
      // replace the following line by a call to your generateShortestPath routine... 
      generateShortestPath(db, source, target);
    }
  }
  
  cout << "Thanks for playing!" << endl;
  return 0;
}

