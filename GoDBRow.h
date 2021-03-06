/*=========================================================================
  Author: $Author: arnaudgelas $  // Author of last commit
  Version: $Rev: 2295 $  // Revision of last commit
  Date: $Date: 2010-10-15 10:20:49 -0400 (Fri, 15 Oct 2010) $  // Date of last commit
=========================================================================*/

/*=========================================================================
 Authors: The GoFigure Dev. Team.
 at Megason Lab, Systems biology, Harvard Medical school, 2009-10

 Copyright (c) 2009-10, President and Fellows of Harvard College.
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice,
 this list of conditions and the following disclaimer.
 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.
 Neither the name of the  President and Fellows of Harvard College
 nor the names of its contributors may be used to endorse or promote
 products derived from this software without specific prior written
 permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
 OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=========================================================================*/

#ifndef __GoDBRow_h
#define __GoDBRow_h

#include "ConvertToStringHelper.h"
#include "vtkMySQLDatabase.h"
#include <string>
#include <sstream>
#include <map>
#include <vector>

//#include "QGoIOConfigure.h"

/**
\class GoDBRow
*/
//class QGOIO_EXPORT GoDBRow
class GoDBRow
{
public:

  typedef std::map< std::string, std::string > StringMapType;
  typedef StringMapType::iterator              StringMapIterator;
  typedef StringMapType::const_iterator        StringMapConstIterator;

  GoDBRow();
  virtual ~GoDBRow();

  template< typename T >
  void SetField(std::string key, T value)
  {
    StringMapIterator it = m_MapRow.find(key);

    if ( it != m_MapRow.end() )
      {
      it->second = ConvertToString< T >(value);
      }
    else
      {
      std::cerr << "This field does not exist!!!" << std::endl;
      }
  }

  /**
  \brief set value as the value of map[key] after having put " at the beginning
  and at the end of the string, as value is a string and it will be needed for
  the database queries */
  void SetField(std::string key, std::string value);

  std::string GetTableName();

  std::string GetTableIDName();

  std::string PrintValues();

  std::string PrintColumnNames();

  std::vector< std::string > GetVectorColumnNames();

  //std::list<std::string> GetListColumnNames();
  std::string PrintColumnNamesWithValues();

  StringMapIterator MapBegin();

  StringMapConstIterator ConstMapBegin();

  StringMapIterator MapEnd();

  StringMapConstIterator ConstMapEnd();

  /**
  \brief return the value for the field map[key] after having removed the "
  at the beginning and at the end of the value if it is a string in order to get
  the original value. */
  std::string GetMapValue(std::string key);

  friend std::ostream & operator<<(std::ostream & os, const GoDBRow & c)
  {
    for ( StringMapConstIterator it = c.m_MapRow.begin();
          it != c.m_MapRow.end();
          ++it )
      {
      os << it->first << " = " << it->second << std::endl;
      }

    return os;
  }

  virtual void SetValuesForSpecificID(int ID, vtkMySQLDatabase *iDatabaseConnector);

protected:
  virtual void InitializeMap() = 0;

  StringMapType m_MapRow;
  std::string   m_TableName;
  std::string   m_TableIDName;
};

#endif
