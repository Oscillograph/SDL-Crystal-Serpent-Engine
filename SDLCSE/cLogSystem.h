#ifndef CLOGSYSTEM_H
#define CLOGSYSTEM_H

// SDLCSE
#include "./common.h"

namespace SDLCSE{
	class cLogSystem{
	public:
		std::ifstream iFile; // input file descriptor
		std::ofstream oFile; // output file descriptor
		std::string commonFile; // where filename is contained
		std::string warningFile; // where filename is contained
		std::string errorFile; // where filename is contained
		std::string fatalFile; // where filename is contained
		std::string sBuffer; // just a buffer for strings
		
		std::stringstream u8stream;
		std::string u8string;
		
		std::vector<std::string> fileContent; // for strings to be arrayed via prepareHighscores???
		
		cLogSystem(){
			commonFile = "log_common.txt";
			warningFile = "log_warnings.txt";
			errorFile = "log_errors.txt";
			fatalFile = "log_fatal.txt";
			
			if (!oFile.is_open()){
				oFile.open(commonFile.c_str(), std::fstream::in | std::fstream::out | std::fstream::trunc);
				oFile << "Logging system initialization...\n";
				oFile.close();
			}
		};
		~cLogSystem(){};
		
		void resetStream(){
			u8string = "";
			u8stream.str(std::string());
		}
		
		template <typename T>
		void u8add(T t) {
			u8stream << t;
		}
		
		template<typename... Args>
		void logStream(Args... args){
			resetStream();
			
			// add args to utf8 string stream
			int dummyLambda[] = { 0, ((void) u8add(std::forward<Args>(args)),0)... };
			
			u8string = u8stream.str();
			
			log(u8string);
		}
		
		void log(std::string message){
			if (!oFile.is_open()){
				oFile.open(commonFile.c_str(), std::fstream::in | std::fstream::out | std::fstream::app);
				oFile << message << "\n";
				oFile.close();
			}
		}
		
		void error(std::string message){
			if (!oFile.is_open()){
				oFile.open(errorFile.c_str(), std::fstream::in | std::fstream::out | std::fstream::app);
				oFile << message << "\n";
				oFile.close();
			}
		}
		
		void warning(std::string message){
			if (!oFile.is_open()){
				oFile.open(warningFile.c_str(), std::fstream::in | std::fstream::out | std::fstream::app);
				oFile << message << "\n";
				oFile.close();
			}
		}
		
		void fatal(std::string message){
			if (!oFile.is_open()){
				oFile.open(fatalFile.c_str(), std::fstream::in | std::fstream::out | std::fstream::app);
				oFile << message << "\n";
				oFile.close();
			}
		}
	};
}

#endif
