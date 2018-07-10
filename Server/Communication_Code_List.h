#pragma once

//Communication Codes:

//---------------------------Action Commands:-------------------------------
#define Move_Player 							((unsigned short) 0)
#define Online_Player_Moved 					((unsigned short) 1)
#define Online_Player_Deleted 					((unsigned short) 3)
#define Daemon_Moved							((unsigned short) 4)
#define Damage_Taken_Player						((unsigned short) 5)
#define Damage_Taken_Daemon						((unsigned short) 6)
#define Healed_Player							((unsigned short) 7)
#define Healed_Daemon							((unsigned short) 8)
#define Attack_Player							((unsigned short) 9)
#define Attack_Daemon							((unsigned short) 10)
#define Mana_Gained_Player						((unsigned short) 11)
#define Mana_Lost_Player						((unsigned short) 12)
#define Die_Player								((unsigned short) 13)
#define Die_Daemon								((unsigned short) 14)
#define Daemon_Deleted							((unsigned short) 15)
#define Running									((unsigned short) 1000)
#define Walking									((unsigned short) 1001)
//--------------------------------------------------------------------------

//------------------------Item Commands:------------------------------------
#define Item_Grabbed							((unsigned short) 2000)
#define Item_Grabbing_Failed					((unsigned short) 2001)
#define Item_Released							((unsigned short) 2002)
#define Item_Releasing_Failed					((unsigned short) 2003)
#define Item_Equipped							((unsigned short) 2004)
#define Item_Equipping_Failed					((unsigned short) 2005)
#define Item_Take_Off							((unsigned short) 2006)
#define Item_Takeing_Off_Failed					((unsigned short) 2007)
#define Item_inInventory						((unsigned short) 2008)
#define Item_inInventory_Failed					((unsigned short) 2009)
#define Item_Auto_Insert						((unsigned short) 2010)
#define Item_Take_Out							((unsigned short) 2011)
#define Item_Takeing_Out_Failed					((unsigned short) 2012)
#define Item_Repositioning						((unsigned short) 2013)
#define Item_Repositioning_Failed				((unsigned short) 2014)
//--------------------------------------------------------------------------

//---------------------------Chat commands:---------------------------------
#define Chat_Message 							((unsigned short) 2500)
//--------------------------------------------------------------------------

//-----------------------Error Messages-------------------------------------
#define Login 									((unsigned short) 3000)
#define Login_failed 							((unsigned short) 3001)
#define Already_logged_in 						((unsigned short) 3002)
#define Kicked_out 								((unsigned short) 3100)
//--------------------------------------------------------------------------

//---------------------------Game Settings----------------------------------

//--------------------------------------------------------------------------

//---------------------------Sends------------------------------------------
#define Send_Player 							((unsigned short) 5000)
#define Send_Wall								((unsigned short) 5001)
#define Send_Walls 								((unsigned short) 5002)
#define Send_Online_Player 						((unsigned short) 5003)
#define Send_Online_Players						((unsigned short) 5004)
#define Send_Item								((unsigned short) 5005)
#define Send_Items								((unsigned short) 5006)
#define Send_Daemons							((unsigned short) 5007)
#define Send_Safezone							((unsigned short) 5008)
#define Send_Safezones							((unsigned short) 5009)			
#define Send_Regenerator						((unsigned short) 5010)
#define Send_Regenerators						((unsigned short) 5011)
#define Send_Respawn							((unsigned short) 5012)
//--------------------------------------------------------------------------