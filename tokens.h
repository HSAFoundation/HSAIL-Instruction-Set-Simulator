enum token_type { TOKEN_CREGISTER= 258 ,
		  TOKEN_DREGISTER,
		  TOKEN_SREGISTER,
		  TOKEN_INTEGER_CONSTANT,
		  TOKEN_SINGLE_CONSTANT,
		  TOKEN_DOUBLE_CONSTANT
				  TOKEN_COMMENT,
				  TOKEN_LOCAL_IDENTIFIER,
				  TOKEN_GLOBAL_IDENTIFIER,
				  TOKEN_LABEL,
				  TOKEN_STRING,
				  TOKEN_PROPERTY,
				  TOKEN_WAVESIZE,
				  
				  /* common keywords */
				  WORKGROUPID,
				  VERSION,
				  GLOBAL,
				  _SAMP,
				  _RWIMG,
				  _ROIMG,
				  ALIGN,
				  _FILE,
				  ARG,
				  KERNARG,
				  FUNCTION,
				  _FBAR,
				  SIGNATURE,
				  BLOCK,
				  ENDBLOCK,
				  BLOCKNUMERIC,
				  BLOCKSTRING,
				  KERNEL,
				  PRAGMA,
				  LABELTARGETS,
				  EXTENSION,
				  EXTERN,
				  STATIC,
				  CONST,
				  PRIVATE,
				  SPILL,
				  GROUP,
				  READONLY,
				  LOC,
				  
				  /* addressSpaceIdentifier keywords */
				  _READONLY,
				  _KERNARG,
				  _GLOBAL,
				  _PRIVATE,
				  _ARG,
				  _GROUP,
				  _SPILL,
				  
				  /* vector*/
				  _V2,
				  _V4,
				  
				  /* init */
				  FORMAT,
				  ORDER,
				  COORD,
				  FILTER,
				  BOUNDARYU,
				  BOUNDARYV,
				  BOUNDARYW
				  
				  
				  
				  

		 };  // begin from 258 to avoid conflict with ASCII characters

