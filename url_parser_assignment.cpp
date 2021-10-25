/*
 * url_parser_assignment.cpp
 * Program Description: This is a program takes a URL as its input and parses it into its constituents and check the validity of the URL
 * Created on: Sep 19, 2021
 * Last Modified: Sep 22, 2021
 * Author: Amanuel Shiferaw
 * Class: CS 3377
 */
//This is a new comment
//this is a very new new comment 
//this is a very very very very new comment
#include <iostream>
#include <string>
#include "version.h"
#include "version.cpp" 

using namespace std;

//main function
int main ()
{ 	
	displayVersion(); 
	//variable index for parsing the URL into its constituents
	//stores the index of the delimiting character
	int protocolEnd,authorityEnd, portStart, parameterStart;

	//boolean value to keep track of whether we have found each respective segments of a URL
	bool foundPort = false;
	bool foundAuthority = false;
	bool foundParameter = false;
	bool foundProtocol = false;

	//take input as a string
	string url;
	cout<<"URL Parser"<<endl;
	cout<<"Please input the URL you want to evaluate:"<<endl;
	cin>>url;

	//traverse the elements in string
	for(int i=0; i<url.length();i++)
	{
		//find the delimiters for protocol and set the index of Protocol End to position containing first ':' followed by two '/' characters
		if(url[i]==':' && url[i+1]=='/' && url[i+2]=='/')
		{
			protocolEnd = i;
			foundProtocol = true;

		}//else if and only if you find the protocol, get the index of the next ':' you encounter and set to Port Start
		else if(url[i] ==':'  && foundProtocol)
		{
			foundPort = true;
			portStart =i;

		}//else if the char is '/' and is not proceeded or is not proceeding by '/' get the index of the character which is the end of the authority
		else if(url[i]=='/' && !foundAuthority && url[i+1]!='/' && url[i-1]!='/' )
		{
			authorityEnd = i;
			foundAuthority = true;

		}//else if it is equal to '?' get the index set to parameter start
		else if(url[i]=='?')
		{
			foundParameter = true;
			parameterStart = i;
		}
	}
	//throw an error if you have not found the protocol or authority(domain+port) since it is a syntactical error
	try
	{
		if(!foundProtocol)
			throw 1;
		if(!foundAuthority)
			throw 2;
	}
	catch (int x)
	{
		if(x==1){
			cout<<"\nURL Syntax Error: Unable to locate the Protocol!"<<endl;
			return 1;
		}
		if(x==2){
			cout<<"\nURL Syntax Error: Unable to locate the Authority!"<<endl;
			return 1;
		}
	}

	//separate the respective elements according to the indexes generated
	string domain, port, filePath, parameter;
	string protocol = url.substr(0,protocolEnd);
	//check if optional port and parameter were found
	//and depending on the result divide the domain and file path
	if(foundPort)
	{
		domain = url.substr(protocolEnd+3,(portStart-(protocolEnd+3)));
		port = url.substr(portStart+1, authorityEnd-(portStart+1));
	}
	else
	{
		domain = url.substr(protocolEnd+3, authorityEnd-(protocolEnd+3));
	}
	if(foundParameter)
	{
		filePath = url.substr(authorityEnd, (parameterStart-authorityEnd));
		parameter = url.substr(parameterStart+1);
	}
	else
	{
		filePath = url.substr(authorityEnd);
	}
	//check if the protocol is valid
	bool protocolCheck;
	if (protocol == "http" || protocol == "https" || protocol == "ftp"|| protocol == "ftps")
		protocolCheck = true;
	else
		protocolCheck = false;

	//check if the host and domain are valid
	int hostIndex;
	int numberOfPeriods = 0;
	for(int i=0; i<domain.length();i++)
	{
		//search for three isolated periods
		if(domain[i] == '.' && domain[i-1]!='.' && domain[i+1]!='.')
		{
			numberOfPeriods++;
			hostIndex = i;
		}
	}

	bool hostCheck = false;
	bool domainCheck = false;
	//the domain has format <aaa>.<bbb>.<ccc> as specified
	//check the formating of domain by checking if there are 2 isolated periods
	if(numberOfPeriods==2)
		domainCheck = true;
	else
		domainCheck = false;

	//check the validity of host in domain
	string host = domain.substr(hostIndex+1);
	if(host == "com" || host =="net"|| host == "edu" || host == "biz"|| host =="gov")
		hostCheck = true;
	else
		hostCheck = false;

	//check if port is valid
	bool portCheck = false;
	int portNum;
	if(foundPort)
	{
		//convert string (port) to an int and
		//check if port contains only digits and not characters
		//for instance "80directory" would be invalid
		portNum = stoi(port);
		string portNumAsString = to_string(portNum);
		try
		{
			//check if there is no loss in conversion to check if they only contain digits
			if(portNumAsString != port)
			{
				throw 3;
			}
		}
		catch(int x)
		{
			//error catching
			cout<<"\nURL Syntax Error: Port number must be an INTEGER between 1 and 65535!"<<endl;
			cout<<"Port is currently "<<port<<endl;
			return 1;
		}

		//check if the port is between required values
		if(portNum>=1 && portNum<=65535)
			portCheck = true;
		else
			portCheck = false;
	}
	//check if the file path is valid according to the instructions
	bool filePathCheck = false;
	int filePathEndIndex;
	for(int i=0; i<filePath.length();i++)
	{
		//find the last period in file path
		if(filePath[i]=='.')
		{
			filePathEndIndex = i;
		}
	}
	//check if the file path end is correct (.wxyz)
	string filePathEnd = filePath.substr(filePathEndIndex);
	if(filePath[0]=='/' && (filePathEnd == ".html"|| filePathEnd == "htm"))
		filePathCheck = true;
	else
		filePathCheck = false;

	//Displaying messages
	//apply demorgan's law for the port(we only want to enter the 'if' if foundPort is true and portCheck is true )
	//or(foundPort is false and portCheck is false)
	//no need to check the parameter since we are sure it begins with '?' and since there were no other condition specified
	if(protocolCheck && domainCheck && hostCheck && filePathCheck && ((!foundPort) || portCheck))
	{
		cout<<"The URL input is VALID!"<<endl;
		cout<<"Protocol:		"<<protocol<<endl;
		cout<<"Domain:			"<<domain<<endl;
		//Display only if port is found
		if(foundPort)
			cout<<"Port:			"<<portNum<<endl;
		cout<<"File Path:		"<<filePath<<endl;
		//Display only if parameter is found
		if(foundParameter)
			cout<<"Parameters:		"<<parameter<<endl;
	}
	else
	{
		//Display error message for the false checks
		cout<<"\nInvalid URL with following erroneous components:\n "<<endl;
		if(!protocolCheck)
			cout<<"Protocol:	"<<protocol<<" is not a valid protocol!"<< endl;
		if(!domainCheck)
			cout<<"Domain:		"<<domain <<" is not a valid domain!"<<endl;
		if(!hostCheck)
			cout<<"Host:		"<<host<<" is an invalid host!"<<endl;
		if(foundPort)
			if(!portCheck)
				cout<<"Port:		port number must be between 1 and 65536!"<<endl;
		if(!filePathCheck)
			cout<<"File Path:	"<<filePath<<" is not a valid file path"<<endl;
	}

	return 0;
}


