#ifndef CHIGHSCORESFILE_H
#define CHIGHSCORESFILE_H

// SDLCSE
#include "./common.h"

namespace SDLCSE{
	/* ------------------
	  This is a highscores file I/O management class
	  ------------------ */
	class cHighScoresFile {
	public:
		cHighScoresFile(){
			MAX_HIGHSCORES = 15;
		}
		~cHighScoresFile(){}
		
		int i, j, k, MAX_HIGHSCORES;
		
		std::ifstream iFile; // input file descriptor
		std::ofstream oFile; // output file descriptor
		std::string filename; // where filename is contained
		std::string sBuffer; // just a buffer for strings
		
		std::vector<std::string> fileContent; // for strings to be arrayed via prepareHighscores???
		std::vector<std::string> vPlayers; // vector of players' names
		std::vector<int> vScores; // vector of scores
		std::vector<int> vIndexes; // indexes to connect players' names and scores
		
		void loadHighScores(std::string filename){
			const char delimWord = '\t'; // delimiter inside one line
			const char delimLine = '\n'; // delimeter between lines
			
			std::string sNick;
			std::string sScore;
			
			// make sure we have no previous data in important vectors
			vPlayers.clear();
			vScores.clear();
			vIndexes.clear();
			
			int pos = 0; // index of a symbol dividing nick and score  
			
			// read contents of the highscores file
			if (!iFile.is_open()){
				iFile.open(filename.c_str());
				if(iFile.is_open()){
					i = 0;
					while (std::getline(iFile, sBuffer, delimLine)){
						pos = sBuffer.find(delimWord);
						sNick = sBuffer.substr(0, pos);
						sScore = sBuffer.substr(pos + 1, std::string::npos);
						
						vPlayers.push_back(sNick);
						vScores.push_back(std::stoi(sScore));
						vIndexes.push_back(i);
						i++;
					}
					iFile.close();
					sortHighScores();
				}
				
				prepareHighScores();
			}
		}
		
		void prepareHighScores(){
			int maxCharacters = 30;
			int currentCharacters, needCharacters = 0;
			std::string str1, str2;
			for (i=0; i<MAX_HIGHSCORES; i++){
				str1 = std::to_string(vScores[i]);
				if (i < 10){
					str2 = std::to_string(i+1) + ". ";
				} else {
					str2 = std::to_string(i+1) + ". ";
				}
				currentCharacters = str2.size() + vPlayers[i].size() + str1.size();
				needCharacters = maxCharacters - currentCharacters;
				
				// add characters we need to make the lines look identical in size
				sBuffer = "";
				for (j=0; j<needCharacters; j++){
					sBuffer = sBuffer + ".";
				}
				sBuffer = str2 + vPlayers[i] + ":" + sBuffer + str1;
				fileContent.push_back(sBuffer);
			}
		}
		
		void inputHighScore(std::string Player, int Score){
			// do this only if we have place for a new highscore or this new highscore has beaten a certain previous one
			if ((vScores.size() < MAX_HIGHSCORES) || (Score > vScores[vScores.size()-1])){
				vPlayers.push_back(Player);
				vScores.push_back(Score);
				vIndexes.push_back(vIndexes.size());
				
				sortHighScores();
			}
		}
		
		void writeHighScores(std::string filename){
			if (!oFile.is_open()){
				oFile.open(filename.c_str());
				if(oFile.is_open()){
					i = 0;
					while (i < MAX_HIGHSCORES){
						sBuffer = vPlayers[i] + "\t" + std::to_string(vScores[i]) + "\n";
						oFile << sBuffer;
						i++;
					}
				}
			}
			oFile.close();
		}
		
		void sortHighScores(){
			int iBuf = 0;
			std::string sBuf;
			
			for (j=0; j<vScores.size(); j++){
				for (k=0; k<vScores.size()-1; k++){
					if (vScores[k] < vScores[k+1]){
						// change positions
						iBuf = vScores[k];
						sBuf = vPlayers[k];
						vScores[k] = vScores[k+1];
						vPlayers[k] = vPlayers[k+1];
						vScores[k+1] = iBuf;
						vPlayers[k+1] = sBuf;
					}
				}
			}
		}
	};
}

#endif
