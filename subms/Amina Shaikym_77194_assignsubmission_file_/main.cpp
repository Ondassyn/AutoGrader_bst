#include <iostream>
#include <string>
#include "set.h"
#include <cmath>
#include <sstream>
#include <algorithm>
#include "mset.h"

int tests_run = 0;
int tests_correct = 0;

bool equalStr(const std::string& s1, const std::string& s2){
	if(s1.size() != s2.size()) return false;
	for(int i = 0; i < s1.size(); i++)
		if(tolower(s1[i]) != tolower(s2[i])) return false;
	return true;
}

void checkString( const std::string s, const std::string&  given, const std::string& correct )
{
   tests_run++;	
   if( !equalStr(given, correct) )
   {
      std::cout << "~INCORRECT " << s << " : " << given <<
                   " SHOULD HAVE BEEN " << correct << "\n";
   } else {
      std::cout << "~CORRECT " << s << " : " << given <<
                   " SHOULD HAVE BEEN " << correct << "\n";
      tests_correct++;
   }
}

void checkBool( const std::string s, const bool&  given, const bool& correct )
{
   tests_run++;	
   if( given != correct )
   {
      std::cout << "~INCORRECT " << s << " : " << given <<
                   " SHOULD HAVE BEEN " << correct << "\n";
   } else {
      std::cout << "~CORRECT " << s << " : " << given <<
                   " SHOULD HAVE BEEN " << correct << "\n";
      tests_correct++;
   }
}

void checkSizet( const std::string s, const size_t&  given, const size_t& correct )
{
   tests_run++;	
   if( given != correct )
   {
      std::cout << "~INCORRECT " << s << " : " << given <<
                   " SHOULD HAVE BEEN " << correct << "\n";
   } else {
      std::cout << "~CORRECT " << s << " : " << given <<
                   " SHOULD HAVE BEEN " << correct << "\n";
      tests_correct++;
   }
}

void checkHeight(const std::string s, const size_t&  given, const size_t& correct){
   tests_run++;	
   if( given != correct && given != correct + 1 )
   {
      std::cout << "~INCORRECT " << s << " : " << given <<
                   " SHOULD HAVE BEEN " << correct << " OR " << correct + 1 << "\n";
   } else {
      std::cout << "~CORRECT " << s << " : " << given <<
                   " SHOULD HAVE BEEN " << correct <<  " OR " << correct + 1 << "\n";
      tests_correct++;
   }
}

void checkDouble( const std::string s, const double& given, const double& correct )
{
   tests_run++;	
   if( std::abs( given - correct ) <= std::abs( correct ) * 1.0E-14 )
   {
      std::cout << "~CORRECT " << s << " : " << given <<
                   " SHOULD HAVE BEEN " << correct << "\n";
      tests_correct++;
   } else {
      std::cout << "~INCORRECT " << s << " : " << given <<
                   " SHOULD HAVE BEEN " << correct << "\n";
   }
}

void checkOutputs( const std::string s, const set& given, const mset& correct ){
	tests_run++;
	std::stringstream ssg;
	std::stringstream ssc;
	ssg << given;
	ssc << correct;
	std::string sg(ssg.str());
	std::string sc(ssc.str());
	
	if(sg.size() != sc.size()){
		std::cout << "~INCORRECT SIZE()" << s << " : " << sg.size() <<
                   " SHOULD HAVE BEEN " << sc.size() << "\n";
        return;
	}
	
	std::string lineg;
	std::string linec;
	while(!ssg.eof() && !ssc.eof()){
		ssg >> lineg;
		ssc >> linec;
		if(!equalStr(lineg, linec)){
			std::cout << "~INCORRECT " << s << " : " << lineg <<
                   " SHOULD HAVE BEEN " << linec << "\n";
       		return;
		}	
	}
	
	std::cout << "~CORRECT " << s << " :\n" << sg << "\n";
    tests_correct++;
}

int main(){

	std::string message = "EQUAL (abcde, abcde)";
	checkBool(message, equal( "abcde", "abcde" ), 1);
	message = "EQUAL (ABCDE, abcde)";
	checkBool(message, equal( "ABCDE", "abcde" ), 1);
	message = "EQUAL (aaaa, aaa)";
	checkBool(message, equal( "aaaa", "aaa" ), 0);
	message = "EQUAL (aaa, aaaa)";
	checkBool(message, equal( "aaa", "aaaa" ), 0);
	message = "EQUAL (abcde, edcba)";
	checkBool(message, equal( "abcde", "edcba" ), 0);
	message = "EQUAL (ab1Cde, ab1cde)";
	checkBool(message, equal( "ab1Cde", "ab1cde" ), 1);
	
	message = "BEFORE (abcde, abcde)";
	checkBool(message, before( "abcde", "abcde" ), 0);
	message = "BEFORE (ABCDE, abcde)";
	checkBool(message, before( "ABCDE", "abcde" ), 0);
	message = "BEFORE (aaaa, aaa)";
	checkBool(message, before( "aaaa", "aaa" ), 0);
	message = "BEFORE (aaa, aaaa)";
	checkBool(message, before( "aaa", "aaaa" ), 1);
	message = "BEFORE (abcde, edcba)";
	checkBool(message, before( "abcde", "edcba" ), 1);
	message = "BEFORE (edcba, abcde)";
	checkBool(message, before( "edcba", "abcde" ), 0);
	
	set s1;
	mset ms1;
	
	message = "OUTPUT ()";
	checkOutputs(message, s1, ms1);
	
	message = "IS_EMPTY()";
	checkBool(message, s1.isempty(), 1);
	message = "SIZE()";
	checkSizet(message, s1.size(), 0);
	message = "HEIGHT ()";
	checkSizet(message, s1.height(), 0);
	message = "INSERT (nolan)";
	checkBool(message, s1.insert("nolan"), 1);
	ms1.insert("nolan");
	message = "IS_EMPTY()";
	checkBool(message, s1.isempty(), 0);
	message = "INSERT (spielberg)";
	checkBool(message, s1.insert("spielberg"), 1);
	ms1.insert("spielberg");
	message = "INSERT (eastwood)";
	checkBool(message, s1.insert("eastwood"), 1);
	ms1.insert("eastwood");
	message = "INSERT (hitchcock)";
	checkBool(message, s1.insert("hitchcock"), 1);
	ms1.insert("hitchcock");
	message = "SIZE()";
	checkSizet(message, s1.size(), 4);
	message = "INSERT (cameron)";
	checkBool(message, s1.insert("cameron"), 1);
	ms1.insert("cameron");
	message = "INSERT (kubrick)";
	checkBool(message, s1.insert("kubrick"), 1);
	ms1.insert("kubrick");
	message = "SIZE()";
	checkSizet(message, s1.size(), 6);
	message = "HEIGHT()";
	checkHeight(message, s1.height(), 3);
	message = "INSERT (easTwooD)";
	checkBool(message, s1.insert("easTwooD"), 0);
	ms1.insert("easTwooD");
	message = "INSERT (cameRon)";
	checkBool(message, s1.insert("cameRon"), 0);
	ms1.insert("cameRon");
	message = "INSERT (kuBricK)";
	checkBool(message, s1.insert("kuBricK"), 0);
	ms1.insert("kuBricK");
	message = "SIZE()";
	checkSizet(message, s1.size(), 6);
	message = "INSERT (Anderson)";
	checkBool(message, s1.insert("Anderson"), 1);
	ms1.insert("Anderson");
	message = "INSERT (cuaron)";
	checkBool(message, s1.insert("cuaron"), 1);
	ms1.insert("cuaron");
	message = "INSERT (tarantino)";
	checkBool(message, s1.insert("TaRantino"), 1);
	ms1.insert("TaRantino");
	message = "SIZE()";
	checkSizet(message, s1.size(), 9);
	message = "HEIGHT()";
	checkHeight(message, s1.height(), 3);
	
	message = "TOP ()";
	checkString(message, s1.top(), "nolan");
	message = "OUTPUT ()";
	checkOutputs(message, s1, ms1);
	
	message = "CONTAINS (SPIELBERG)";
	checkBool(message, s1.contains("SPIELBERG"), 1);
	message = "CONTAINS (COPPOLA)";
	checkBool(message, s1.contains("COPPOLA"), 0);
	message = "CONTAINS (hitchcock)";
	checkBool(message, s1.contains("hitchcock"), 1);
	message = "CONTAINS (cameroni)";
	checkBool(message, s1.contains("cameroni"), 0);
	
	message = "CONTAINS (SPIELBERG)";
	checkBool(message, s1.contains("SPIELBERG"), 1);
	message = "CONTAINS (hitchcock)";
	checkBool(message, s1.contains("hitchcock"), 1);	

	message = "INSERT (TaRaNtINo)";
	checkBool(message, s1.insert("TaRaNtINo"), 0);
	ms1.insert("TaRaNtINo");
	message = "INSERT (zemeckis)";
	checkBool(message, s1.insert("zemeckis"), 1);
	ms1.insert("zemeckis");
	message = "INSERT (wachowski)";
	checkBool(message, s1.insert("wachowski"), 1);
	ms1.insert("wachowski");
	message = "SIZE()";
	checkSizet(message, s1.size(), 11);
	message = "HEIGHT()";
	checkHeight(message, s1.height(), 4);
	message = "OUTPUT ()";
	checkOutputs(message, s1, ms1);
	
	message = "REMOVE (zemeckis)";
	checkBool(message, s1.remove("zemeckis"), 1);
	ms1.remove("zemeckis");
	message = "SIZE()";
	checkSizet(message, s1.size(), 10);
	message = "HEIGHT()";
	checkHeight(message, s1.height(), 3);
	message = "REMOVE (kurosawa)";
	checkBool(message, s1.remove("kurosawa"), 0);
	ms1.remove("kurosawa");
	message = "REMOVE (wachowski)";
	checkBool(message, s1.remove("wachowski"), 1);
	ms1.remove("wachowski");
	checkBool(message, s1.remove("kubrick"), 1);
	ms1.remove("kubrick");
	message = "SIZE()";
	checkSizet(message, s1.size(), 8);
	message = "HEIGHT()";
	checkHeight(message, s1.height(), 3);
	checkBool(message, s1.remove("nolan"), 1);
	ms1.remove("nolan");
	message = "TOP ()";
	checkString(message, s1.top(), "eastwood");
	message = "SIZE()";
	checkSizet(message, s1.size(), 7);
	message = "HEIGHT()";
	checkHeight(message, s1.height(), 3);
	message = "OUTPUT ()";
	checkOutputs(message, s1, ms1);
	checkBool(message, s1.remove("eastwood"), 1);
	ms1.remove("eastwood");
	message = "TOP ()";
	checkString(message, s1.top(), "cameron");
	message = "SIZE()";
	checkSizet(message, s1.size(), 6);
	message = "HEIGHT()";
	checkHeight(message, s1.height(), 4);
	message = "OUTPUT ()";
	checkOutputs(message, s1, ms1);
	
	s1.clear();
	ms1.clear();
	message = "SIZE()";
	checkSizet(message, s1.size(), 0);
	message = "HEIGHT()";
	checkSizet(message, s1.height(), 0);
	
	
	message = "INSERT (Lee)";
	checkBool(message, s1.insert("Lee"), 1);
	ms1.insert("Lee");	
	message = "SIZE()";
	checkSizet(message, s1.size(), 1);
	message = "HEIGHT()";
	checkHeight(message, s1.height(), 0);
	message = "INSERT (finCHER)";
	checkBool(message, s1.insert("finCHER"), 1);
	ms1.insert("finCHER");
	message = "INSERT (scorcese)";
	checkBool(message, s1.insert("scorcese"), 1);
	ms1.insert("scorcese");
	message = "SIZE()";
	checkSizet(message, s1.size(), 3);
	message = "HEIGHT()";
	checkHeight(message, s1.height(), 1);
	message = "REMOVE (tarkovskiy)";
	checkBool(message, s1.remove("tarkovskiy"), 0);
	ms1.remove("tarkovskiy");
	message = "REMOVE (LEE)";
	checkBool(message, s1.remove("LEE"), 1);
	ms1.remove("LEE");
	message = "SIZE()";
	checkSizet(message, s1.size(), 2);
	message = "HEIGHT()";
	checkHeight(message, s1.height(), 1);
	message = "OUTPUT ()";
	checkOutputs(message, s1, ms1);
	
	std::cout << "\nTESTS RUN: " << tests_run << std::endl;
	std::cout << "\nTESTS CORRECT: " << tests_correct << std::endl;		
	return 0;
}
