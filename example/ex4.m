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

