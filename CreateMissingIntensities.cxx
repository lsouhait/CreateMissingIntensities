#include "vtkMySQLDatabase.h"
#include "vtkSQLQuery.h"
#include "vtkStdString.h"
#include "vtkVariant.h"
#include <sstream>
#include <string>

#include "vtkMySQLDatabase.h"
#include "SelectQueryDatabaseHelper.h"
#include "QueryDataBaseHelper.h"
#include "GoDBIntensityRow.h"
#include "GoDBRow.h"


int main(int argc, char *argv[])
{
  if (argc != 2)
    {
    std::cerr << "Script to create missing intensities requires the mysql root password as argument:" << std::endl;
    return EXIT_FAILURE;
    }

  std::pair< bool, vtkMySQLDatabase * > ConnectionDatabase = ConnectToDatabase(
    "localhost", "root", argv[1], "gofiguredatabase");

  if ( !ConnectionDatabase.first )
    {
    std::cout << "No connection open for QGoOpenOrCreateImgSession" << std::endl;
    std::cout << "Debug: In " << __FILE__ << ", line " << __LINE__;
    std::cout << std::endl;
    return EXIT_FAILURE;
    }
  vtkMySQLDatabase* DatabaseConnector = ConnectionDatabase.second;

  std::stringstream Query;
  Query << "select imagingsessionid,mesh.meshid ";
  Query << "from mesh left join intensity on mesh.meshid = intensity.meshid ";
  Query << "where (points <> '0' and mesh.meshid = intensity.meshid is null) ";
  Query << "order by imagingsessionid asc";
  std::vector<std::string> ListImgIDsMeshIDsWithProblem = 
    ExecuteSelectQuery( DatabaseConnector, Query.str() );

  if (!ListImgIDsMeshIDsWithProblem.empty())
    {
    std::vector<std::string>::iterator iter = ListImgIDsMeshIDsWithProblem.begin();
    while(iter != ListImgIDsMeshIDsWithProblem.end()-2)
      {
      std::string ImgID = *iter;
      std::stringstream QueryStream;
      QueryStream << "select channelid from channel where imagingsessionid = ";
      QueryStream << ImgID;
      std::vector<std::string> ChannelIDs = ExecuteSelectQuery( DatabaseConnector, QueryStream.str());
      while (*iter == ImgID )//for each imagingsessionid
        {
        iter++;
        std::string MeshID = *iter; //for each meshid
        std::vector<std::string>::iterator iterChannel = ChannelIDs.begin();
        while(iterChannel != ChannelIDs.end())//for each channel
          {
          std::string ChannelID = *iterChannel;
          GoDBIntensityRow NewIntensity;
          NewIntensity.SetField("Value","0");
          NewIntensity.SetField("meshID",MeshID);
          NewIntensity.SetField("ChannelID",ChannelID);
          NewIntensity.SaveInDB(DatabaseConnector);
          iterChannel++;
          }//end while channel
        iter++;
        }//end while imagingsession
      }//end while ListImgIDsMeshIDsWithProblem
    //for the last one:
     std::string ImgID = *iter;
     std::stringstream QueryStream;
     QueryStream << "select channelid from channel where imagingsessionid = ";
     QueryStream << ImgID;
     std::vector<std::string> ChannelIDs = ExecuteSelectQuery( DatabaseConnector, QueryStream.str());
     iter++;
     std::string MeshID = *iter; //for each meshid
     std::vector<std::string>::iterator iterChannel = ChannelIDs.begin();
     while(iterChannel != ChannelIDs.end())//for each channel
      {
      std::string ChannelID = *iterChannel;
      GoDBIntensityRow NewIntensity;
      NewIntensity.SetField("Value","0");
      NewIntensity.SetField("meshID",MeshID);
      NewIntensity.SetField("ChannelID",ChannelID);
      NewIntensity.SaveInDB(DatabaseConnector);
      iterChannel++;
      }//end while channel
    }// end if 

  //close and delete DatabaseConnector:
  CloseDatabaseConnection(DatabaseConnector);
}


