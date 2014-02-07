#include <bass.h>
void Set_Master_Volume(float volume)
{
	if(volume>1)
	{
		//BASS_SetVolume(volume/99.f);
	}
	else
	{
		//BASS_SetVolume(volume);
	}
}

#include <Windows.h>
#include <string>

bool dirExists(const std::string& dirName_in)
{
  DWORD ftyp = GetFileAttributesA(dirName_in.c_str());
  if (ftyp == INVALID_FILE_ATTRIBUTES)
    return false;  //something is wrong with your path!

  if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
    return true;   // this is a directory!

  return false;    // this is not a directory!
}