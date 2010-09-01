/************************************************************************
**
**  Copyright (C) 2010  Strahinja Markovic
**
**  This file is part of FlightCrew.
**
**  FlightCrew is free software: you can redistribute it and/or modify
**  it under the terms of the GNU Lesser General Public License as published
**  by the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  FlightCrew is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU Lesser General Public License for more details.
**
**  You should have received a copy of the GNU Lesser General Public License
**  along with FlightCrew.  If not, see <http://www.gnu.org/licenses/>.
**
*************************************************************************/

#include <stdafx.h>
#include "SatisfiesXhtmlSchema.h"
#include "Misc/ErrorResultCollector.h"
#include <ToXercesStringConverter.h>
#include <XmlUtils.h>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <LocationAwareDOMParser.h>

namespace FlightCrew
{

std::vector<Result> FlightCrew::SatisfiesXhtmlSchema::ValidateFile( const fs::path &filepath )
{
    xe::LocationAwareDOMParser parser;

    parser.setDoSchema( true );
    parser.setLoadSchema( false );
    parser.setSkipDTDValidation( true );

    parser.setDoNamespaces( true );
    parser.setValidationScheme( xc::AbstractDOMParser::Val_Always );
    parser.useCachedGrammarInParse( true );    

    xc::MemBufInputSource dtd( XHTML11_FLAT_DTD,
                               XHTML11_FLAT_DTD_LEN,
                               toX( XHTML11_FLAT_DTD_ID ) );        

    xc::MemBufInputSource ops( OPS201_XSD,
                               OPS201_XSD_LEN,
                               toX( OPS201_XSD_ID ) );

    xc::MemBufInputSource ops_switch( OPS_SWITCH_XSD, 
                                      OPS_SWITCH_XSD_LEN,
                                      toX( OPS_SWITCH_XSD_ID ) );      

    xc::MemBufInputSource svg( SVG11_XSD, 
                               SVG11_XSD_LEN,  
                               toX( SVG11_XSD_ID ) );      

    xc::MemBufInputSource xlink( XLINK_XSD, 
                                 XLINK_XSD_LEN,
                                 toX( XLINK_XSD_ID ) );       

    xc::MemBufInputSource xml( XML_XSD, 
                               XML_XSD_LEN,
                               toX( XML_XSD_ID  ) );      

    parser.loadGrammar( dtd,        xc::Grammar::DTDGrammarType,    true );
    parser.loadGrammar( xml,        xc::Grammar::SchemaGrammarType, true );
    parser.loadGrammar( xlink,      xc::Grammar::SchemaGrammarType, true );
    parser.loadGrammar( svg,        xc::Grammar::SchemaGrammarType, true );
    parser.loadGrammar( ops_switch, xc::Grammar::SchemaGrammarType, true );
    parser.loadGrammar( ops,        xc::Grammar::SchemaGrammarType, true );

    parser.setExternalSchemaLocation( "http://www.w3.org/1999/xhtml ops201.xsd" );

    ErrorResultCollector collector;
    parser.setErrorHandler( &collector );

    parser.parse( filepath.string().c_str() );

    return collector.GetResults();
}

} //namespace FlightCrew