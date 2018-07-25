#pragma once

//Communication Codes:

//---------------------------Action Commands:-------------------------------
static const unsigned short Move_Player =								0;
static const unsigned short Online_Player_Moved =						1;
static const unsigned short Online_Player_Deleted =						3;
static const unsigned short Daemon_Moved =								4;
static const unsigned short Damage_Taken_Player =						5;
static const unsigned short Damage_Taken_Daemon =						6;
static const unsigned short Healed_Player =								7;
static const unsigned short Healed_Daemon =								8;
static const unsigned short Attack_Player =								9;
static const unsigned short Attack_Daemon =								10;
static const unsigned short Mana_Gained_Player =						11;
static const unsigned short Mana_Lost_Player =							12;
static const unsigned short Die_Player =								13;
static const unsigned short Die_Daemon =								14;
static const unsigned short Daemon_Deleted =							15;
static const unsigned short Running =									1000;
static const unsigned short Walking =									1001;
//--------------------------------------------------------------------------

//------------------------Item Commands:------------------------------------
static const unsigned short Item_Grabbed =								2000;
static const unsigned short Item_Grabbing_Failed =						2001;
static const unsigned short Item_Released =								2002;
static const unsigned short Item_Releasing_Failed =						2003;
static const unsigned short Item_Equipped =								2004;
static const unsigned short Item_Equipping_Failed =						2005;
static const unsigned short Item_Take_Off =								2006;
static const unsigned short Item_Takeing_Off_Failed =					2007;
static const unsigned short Item_inInventory =							2008;
static const unsigned short Item_inInventory_Failed =					2009;
static const unsigned short Item_Auto_Insert =							2010;
static const unsigned short Item_Take_Out =								2011;
static const unsigned short Item_Takeing_Out_Failed =					2012;
static const unsigned short Item_Repositioning =						2013;
static const unsigned short Item_Repositioning_Failed =					2014;
//--------------------------------------------------------------------------

//---------------------------Chat commands:---------------------------------
static const unsigned short Chat_Message =								2500;
//--------------------------------------------------------------------------

//-----------------------Error Messages-------------------------------------
static const unsigned short Login =										3000;
static const unsigned short Login_failed =								3001;
static const unsigned short Already_logged_in =							3002;
static const unsigned short Kicked_out =								3100;
//--------------------------------------------------------------------------

//---------------------------Game Settings----------------------------------

//--------------------------------------------------------------------------

//---------------------------Sends------------------------------------------
static const unsigned short Send_Player =								5000;
static const unsigned short Send_Wall =									5001;
static const unsigned short Send_Walls =								5002;
static const unsigned short Send_Online_Player =						5003;
static const unsigned short Send_Online_Players =						5004;
static const unsigned short Send_Item =									5005;
static const unsigned short Send_Items =								5006;
static const unsigned short Send_Daemons =								5007;
static const unsigned short Send_Safezone =								5008;
static const unsigned short Send_Safezones =							5009;
static const unsigned short Send_Regenerator =							5010;
static const unsigned short Send_Regenerators =							5011;
static const unsigned short Send_Respawn =								5012;
//--------------------------------------------------------------------------