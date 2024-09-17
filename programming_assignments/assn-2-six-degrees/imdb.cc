using namespace std;
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include "imdb.h"
#include <cstring>
#include <algorithm>

const char *const imdb::kActorFileName = "actordata";
const char *const imdb::kMovieFileName = "moviedata";

imdb::imdb(const string& directory)
{
  const string actorFileName = directory + "/" + kActorFileName;
  const string movieFileName = directory + "/" + kMovieFileName;
  
  actorFile = acquireFileMap(actorFileName, actorInfo);
  movieFile = acquireFileMap(movieFileName, movieInfo);
}

bool imdb::good() const
{
  return !( (actorInfo.fd == -1) || 
	    (movieInfo.fd == -1) ); 
}

// you should be implementing these two methods right here... 
bool imdb::getCredits(const string& player, vector<film>& films) const {
  //do binary search on array of ints to get offset bytes
  int* arr_start = (int*)actorFile + 1;
  int size = *(int*) actorFile;
  int* arr_end = arr_start + size;
  int* offset = lower_bound(arr_start, arr_end, player.c_str(), [this](auto const& offset, auto const& key) {
    return strcmp((char*)this->actorFile + offset, (char*) key) < 0;
  });
  if (offset == arr_end) return false;
  //go to films array and for each int there return film name string at film array + offset bytes
  //actor record beginning
  char* actor = (char*)actorFile + * offset;
  int nameLen = strlen(actor) + 1;
  nameLen += (nameLen%2);
  short* filmCount_ptr = (short*) (actor + nameLen);
  short filmCount = *filmCount_ptr;
  int* filmArrayOffsets = (int*) (filmCount_ptr + 1);
  filmArrayOffsets = (int*) ((char*)filmArrayOffsets + (unsigned long long)filmArrayOffsets%4);
  for (short i = 0; i < filmCount; i++) {
    char* title = (char*)movieFile + filmArrayOffsets[i];
    int titleLen = strlen(title) + 1;
    int year = 1900 + *(char*) (title + titleLen);
    film f1;
    f1.title = title;
    f1.year = year;
    films.push_back(f1);
  }
  return films.size() > 0;
}
bool imdb::getCast(const film& movie, vector<string>& players) const {
  string title = movie.title;
  int* arr_start = (int*)movieFile + 1;
  int size = *(int*) movieFile;
  int* arr_end = arr_start + size;
  int* offset = lower_bound(arr_start, arr_end, movie, [this](auto const& offset, auto const& key) {
    char* title = (char*)this->movieFile + offset;
    int year = 1900 + *(char*) (title + strlen(title) + 1);
    film elem;
    elem.title = title;
    elem.year = year;
    return elem < key;
  });
  if (offset == arr_end) return false;
  char* film = (char*)movieFile + *offset;
  //traverse the offset array
  int byteCount = strlen(film) + 2; //1 byte for \0 and 1 byte for year
  short padding = byteCount%2;
  short* actorsCount_ptr = (short*) ((char*)film + byteCount + padding);
  short actorsCount = *actorsCount_ptr;
  padding = ((unsigned long long) actorsCount_ptr + sizeof(short) ) % 4;
  int* actorsOffsetArray = (int*) ((char*)actorsCount_ptr + sizeof(short) + padding);
  for (short i = 0; i < actorsCount; i++) {
    string actor = (char*)actorFile + actorsOffsetArray[i];
    players.push_back(actor);
  }
  return players.size() > 0;
}

imdb::~imdb()
{
  releaseFileMap(actorInfo);
  releaseFileMap(movieInfo);
}

// ignore everything below... it's all UNIXy stuff in place to make a file look like
// an array of bytes in RAM.. 
const void *imdb::acquireFileMap(const string& fileName, struct fileInfo& info)
{
  struct stat stats;
  stat(fileName.c_str(), &stats);
  info.fileSize = stats.st_size;
  info.fd = open(fileName.c_str(), O_RDONLY);
  return info.fileMap = mmap(0, info.fileSize, PROT_READ, MAP_SHARED, info.fd, 0);
}

void imdb::releaseFileMap(struct fileInfo& info)
{
  if (info.fileMap != NULL) munmap((char *) info.fileMap, info.fileSize);
  if (info.fd != -1) close(info.fd);
}
