/* File:      FlrException.java
**
** Author(s): Aditi Pandit
**
** Contact:   see  ../CONTACTS.txt
** 
** Copyright (C) The Research Foundation of SUNY, 2005-2018.
** 
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**      http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
**
**
** 
*/

// TODO:  make this a real Exception

package net.sf.flora2.API.util;

/** 
 * An Exception related to Flora processing in general.
 * This includes exceptions returned by Flora of the form error(Error, Message, Backtrace).
 */
public class FlrException extends RuntimeException{
    private static final long serialVersionUID = 2;

    /**
     * Flora error.
     */
    Object error;

    /**
     * Flora message.
     */
    Object floraMessage;

    /**
     * XSB backtrace (a list of list of large integers).
     */
    Object backtrace;
    
    public FlrException(String message){
	super(message);
    }

    public FlrException(String message, Throwable cause){
	super(message, cause);
    }

    /**
     * Constructs a FlrException from the components of an exception returned by Flora.
     * @param error
     * @param floraMessage
     * @param backtrace
     */
    public FlrException(Object error, Object floraMessage, Object backtrace)
    {
    	super(error + ": " + floraMessage);
    	this.error = error;
    	this.floraMessage = floraMessage;
    	this.backtrace = backtrace;
    }

    public Object getError()
    {
    	return error;
    }

    public Object getFloraMessage()
    {
    	return floraMessage;
    }
    
    public Object getBacktrace()
    {
    	return backtrace;
    }
}
