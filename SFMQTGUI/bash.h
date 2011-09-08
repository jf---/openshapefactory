/************************************************************************************
**
** This file is part of the Flexible Digital Modeller
**
** Copyright (C) 2010,2011 Alexander Peña de Leon
**
**This work is licensed under the Creative Commons 
**Attribution-NonCommercial-ShareAlike 3.0 Australia License. 
**To view a copy of this license, 
**visit http://creativecommons.org/licenses/by-nc-sa/3.0/au/ 
**or send a letter to Creative Commons, 171 Second Street, 
**Suite 300, San Francisco, California, 94105, USA.
**
***********************************************************************************/


/*
 *	UNIX shell -----> BUTCHERED CODE BY ALEX PENA
 *
 *	S. R. Bourne----> STANDING ON GIANTS SHOLDER:ALEX PENA
 *	Bell Telephone Laboratories ////////\\ SIAL RMIT UNIVERSITY FEB 25 2011


 *  Alex Notes: the Bourne Shell was written by making c preprocessor macros this is black belt stuff.
 WE ARE GOING TO USE THIS TO BUILD A BASIC LANGUAGE FOR BEGINNERS, PROGRAMMING TRAINING WHEELS, THEY SHOW THE MAIN CONCEPTS OF PROGRAMMING AS TOKENS REPLACING THE MEANING OF THE SYNTAX WITH ENGLISH WORDS, THIS PROVES THAT SYNTAX IS A UNECESSARY EVIL, AND THAT IF THE LANGUAGE IS NOT EXPRESSIVE ON ITS OWN MERITS THEN IT DOESNT MATTER WHAT SYNTAX YOU USE.
 */

#define LOCAL	static
#define PROC	extern
#define TYPE	typedef
#define STRUCT	TYPE struct
#define UNION	TYPE union
#define REG	register

#define IF	if(
#define THEN	){
#define ELSE	} else {
#define ELSEIF	} else if (
#define ENDIF	;}

#define BEGIN	{
#define END	}
#define SWITCH	switch(
#define IN	){
#define ENDSWITCH	}
#define FOR	for(

#define FOR(name,start,end)	for(int name = start;name < end;name = name +1){\

#define WHILE	while(
#define DO	){
#define ENDDO	;}
#define REP	do{
#define PER	}while(
#define DONE	);
#define LOOP	for(;;){
#define ENDLOOP	}


#define SKIP	;
#define DIV	/
#define REM	%
#define NEQ	^
#define AND	&&
#define OR	||
#define NOT(expression) !expression

#define TRUE	(-1)
#define FALSE	0
#define LOBYTE	0377
#define STRIP	0177
#define QUOTE	0200

#define EOF	0
#define NL	'\n'
#define SP	' '
#define LQ	'`'
#define RQ	'\''
#define MINUS	'-'
#define COLON	':'

#define MAX(a,b)	((a)>(b)?(a):(b))
