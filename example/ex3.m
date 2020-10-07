v4wNode() ; 0.19.0 ; Aug 08, 2020@20:21
 ;
 ; Package:    NodeM
 ; File:       v4wNode.m
 ; Summary:    Call-in integration routine
 ; Maintainer: David Wicksell <dlw@linux.com>
 ;
 ; Written by David Wicksell <dlw@linux.com>
 ; Copyright © 2012-2020 Fourth Watch Software LC
 ;
 ; This program is free software: you can redistribute it and/or modify
 ; it under the terms of the GNU Affero General Public License (AGPL)
 ; as published by the Free Software Foundation, either version 3 of
 ; the License, or (at your option) any later version.
 ;
 ; This program is distributed in the hope that it will be useful,
 ; but WITHOUT ANY WARRANTY; without even the implied warranty of
 ; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 ; GNU Affero General Public License for more details.
 ;
 ; You should have received a copy of the GNU Affero General Public License
 ; along with this program. If not, see http://www.gnu.org/licenses/.
 ;
 ; NOTE: Although this routine can be called directly, it is not a good idea; it
 ; is hard to use, and very clunky. It is written with the sole purpose of
 ; providing software integration between NodeM's C code and M code, via the
 ; YottaDB/GT.M Call-in interface. Although each function is documented, that is
 ; for internal maintenance and testing purposes, and there are no plans for any
 ; API usage documentation in the future.
 ;
 quit:$quit "Call an API entry point" write "Call an API entry point",! quit
 ;
 ;; @function {private} isNumber
 ;; @summary Returns true if data is number, false if not
 ;; @param {string} data - Input data to be tested; a single subscript, function or procedure argument, or data
 ;; @param {string} direction (input|output) - Processing control direction
 ;; @returns {number} (0|1) - Return code representing data type
isNumber:(data,direction)
 ; YottaDB/GT.M approximate (using number of digits, rather than number value) number limits:
 ;   - 47 digits before overflow (resulting in an overflow error)
 ;   - 18 digits of precision
 ; Node.js/JavaScript approximate (using number of digits, rather than number value) number limits:
 ;   - 309 digits before overflow (represented as the Infinity primitive)
 ;   - 21 digits before conversion to exponent notation
 ;   - 16 digits of precision
 if $get(v4wDebug,0)>2 do debugLog(">>>    isNumber enter:") zwrite data,direction
 ;
 if data'["E",data=+data do:$get(v4wDebug,0)>2 debugLog(">>>    isNumber: 1") quit 1
 else  if direction="input",data?.1"-"1"0"1"."1.N  do:$get(v4wDebug,0)>2 debugLog(">>>    isNumber: 1") quit 1
 else  do:$get(v4wDebug,0)>2 debugLog(">>>    isNumber: 0") quit 0
 ;; @end isNumber function
 ;
 ;; @function {private} isString
 ;; @summary Returns true if data is string (including whether it has surrounding quotes), false if not
 ;; @param {string} data - Input data to be tested; a single subscript, function or procedure argument, or data
 ;; @param {string} direction (input|output) - Processing control direction
 ;; @returns {number} (0|1|2) - Return code representing data type
isString:(data,direction)
 ; The note under isNumber is why anything over 16 characters needs to be treated as a string
 if $get(v4wDebug,0)>2 do debugLog(">>>    isString enter:") zwrite data,direction
 ;
 if ($zextract(data)="""")&($zextract(data,$zlength(data))="""") do:$get(v4wDebug,0)>2 debugLog(">>>    isString: 3") quit 3
 else  if $zlength(data)>16 do:$get(v4wDebug,0)>2 debugLog(">>>    isString: 1") quit 1
 else  if (direction="input")&((data["e+")!(data["e-")) do:$get(v4wDebug,0)>2 debugLog(">>>    isString: 1") quit 1
 else  if $$isNumber(data,direction) do:$get(v4wDebug,0)>2 debugLog(">>>    isString: 0") quit 0
 else  do:$get(v4wDebug,0)>2 debugLog(">>>    isString: 2") quit 2
 ;; @end isString function
