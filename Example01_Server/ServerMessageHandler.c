//
//  ServerMessageHandler.c
//  Ex04_Server
//
//  Created by Dean Zahavy on 26/12/2019.
//  Copyright © 2019 Dean Zahavy. All rights reserved.
//

#pragma region Includes

#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include <stdlib.h>
#include "ServerMessageHandler.h"
#include "SocketSendRecvTools.h"
#include "ServerHadnler.h"
#include "LeaderBoarrd.h"
#include "MessegeHead.h"
#pragma endregion

#pragma region SendMessageFunctions
int sendGeneralMesseage(char* messageID, SOCKET * sd)
{
	char* mssg = calloc(1, strlen(messageID) + BUFFER);

	if (mssg == NULL)
	{
		return (NULL);
	}

	mssg = strcat(mssg, messageID);

	// send mssg.			
	TransferResult_t SendRes = SendString(mssg, sd);
	free(mssg);

	if (SendRes == TRNS_FAILED)
	{
		printf("Service socket error while writing, closing thread.\n");
		closesocket(sd);
		return ERROR_IN_CONNECTION;
	}

	return(NO_ERROR1);
}


int sendServerDenieMessage(char* messageID, char* message, SOCKET * sd)
{
	char* mssgDenie = calloc(1, strlen(messageID) + strlen(message) + BUFFER);

	if (mssgDenie == NULL)
	{
		return (MALLOC_ERROR);
	}

	mssgDenie = strcat(mssgDenie, messageID);
	mssgDenie = strcat(mssgDenie, PARAMETER_BEGIN_CHAR);
	mssgDenie = strcat(mssgDenie, message);

	// send mssg.			
	TransferResult_t SendRes = SendString(mssgDenie, sd);
	free(mssgDenie);

	if (SendRes == TRNS_FAILED)
	{
		printf("Service socket error while writing, closing thread.\n");
		closesocket(sd);
		return ERROR_IN_CONNECTION;
	}

	return(NO_ERROR1);
}

int sendServerInvite(char* messageID, char* name, SOCKET * sd)
{
	char* mssg = calloc(1, strlen(messageID) + strlen(name) + BUFFER);

	if (mssg == NULL)
	{
		return (MALLOC_ERROR);
	}

	mssg = strcat(mssg, messageID);
	mssg = strcat(mssg, PARAMETER_BEGIN_CHAR);
	mssg = strcat(mssg, name);

	// send mssg.			
	TransferResult_t SendRes = SendString(mssg, sd);
	free(mssg);

	if (SendRes == TRNS_FAILED)
	{
		printf("Service socket error while writing, closing thread.\n");
		closesocket(sd);
		return ERROR_IN_CONNECTION;
	}

	return(NO_ERROR1);
}

int sendGameResultMessage(char* messageID, char* client, char* moveOp, char* moveMe, char* won, SOCKET * sd)
{
	char* mssg = calloc(1, strlen(messageID) + strlen(client) + strlen(moveOp)
		+ strlen(moveMe) + strlen(won) + BUFFER);

	if (mssg == NULL)
	{
		return (MALLOC_ERROR);
	}

	mssg = strcat(mssg, messageID);
	mssg = strcat(mssg, PARAMETER_BEGIN_CHAR);
	mssg = strcat(mssg, client);
	mssg = strcat(mssg, PARAMETER_SPLIT_CHAR);
	mssg = strcat(mssg, moveOp);
	mssg = strcat(mssg, PARAMETER_SPLIT_CHAR);
	mssg = strcat(mssg, moveMe);
	mssg = strcat(mssg, PARAMETER_SPLIT_CHAR);
	mssg = strcat(mssg, won);

	// send mssg.			
	TransferResult_t SendRes = SendString(mssg, sd);
	free(mssg);

	if (SendRes == TRNS_FAILED)
	{
		printf("Service socket error while writing, closing thread.\n");
		closesocket(sd);
		return ERROR_IN_CONNECTION;
	}

	return(NO_ERROR1);
}

int sendOponnentQuitMessage(char* messageID, char* otherClient, SOCKET * sd)
{
	char* mssg = calloc(1, strlen(messageID) + strlen(otherClient) + BUFFER);

	if (mssg == NULL)
	{
		return (MALLOC_ERROR);
	}

	mssg = strcat(mssg, messageID);
	mssg = strcat(mssg, PARAMETER_BEGIN_CHAR);
	mssg = strcat(mssg, otherClient);

	// send mssg.			
	TransferResult_t SendRes = SendString(mssg, sd);
	free(mssg);

	if (SendRes == TRNS_FAILED)
	{
		printf("Service socket error while writing, closing thread.\n");
		closesocket(sd);
		return ERROR_IN_CONNECTION;
	}

	return(NO_ERROR1);
}

int sendLeaderBoardMessage(char* messageID, char* leaderFileContent, SOCKET * sd)
{
	char* mssg = calloc(1, strlen(messageID) + strlen(leaderFileContent) + BUFFER);

	if (mssg == NULL)
	{
		return (MALLOC_ERROR);
	}

	mssg = strcat(mssg, messageID);
	mssg = strcat(mssg, PARAMETER_BEGIN_CHAR);
	mssg = strcat(mssg, leaderFileContent);

	// send mssg.			
	TransferResult_t SendRes = SendString(mssg, sd);
	free(mssg);

	if (SendRes == TRNS_FAILED)
	{
		printf("Service socket error while writing, closing thread.\n");
		closesocket(sd);
		return ERROR_IN_CONNECTION;
	}

	return(NO_ERROR1);
}
#pragma endregion

#pragma region Pharseing Functions

int pharseClientRequest(char* name, SockParams * param)
{
	int result = NO_ERROR1;
	if (isLocationAvilableForClient() == TRUE_VAL)
	{
		changeName(name);
		result = sendGeneralMesseage(SERVER_APPROVED, *param->sd);
		result = sendGeneralMesseage(SERVER_MAIN_MENU, *param->sd);
		increaseCountLogged();
	}
	else
	{
		result = sendServerDenieMessage(SERVER_DENIED, SERVER_DENIED_MESSAGE, *param->sd);

		if (result != NO_ERROR1)
		{
			return(result);
		}
		else
		{
			return(SERVER_DENIE_CLIENT);
		}
	}
	
	return (result);
}

int pharseClientMainMenue(SockParams * param)
{
	int result = sendGeneralMesseage(SERVER_MAIN_MENU, *param->sd);

	return (NO_ERROR1);
}

int pharseClientCPU(SockParams * param)
{
	int result = sendGeneralMesseage(SERVER_PLAYER_MOVE_REQUEST, *param->sd);
	return(NO_ERROR1);
}

int pharseClientVS(SockParams * param)
{
	printf("\nyou wanna fight here?!\n");
	return(NO_ERROR1);
}

int pharseClientLeader(SockParams * param, int isUpdate)
{
	char* leaderFile = NULL;

	if (isUpdate == FALSE_VAL)
	{
		leaderFile = getLeaderInstanseFileFormat();
	}
	else
	{
		leaderFile = calloc(1,1);
	}

	if (leaderFile == NULL)
	{
		return (MALLOC_ERROR);
	}

	int result = sendLeaderBoardMessage(SERVER_LEADERBOARD, leaderFile, *param->sd);

	free(leaderFile);

	// Create Leader Menue.
	result = sendGeneralMesseage(SERVER_LEADERBORAD_MENU, *param->sd);

	return(NO_ERROR1);
}

int pharseClientMove(char* move, SockParams * param)
{
	// Get cpu move.
	char* cpu = getRandMove();

	// Check who won.
	char* won = checkWin(move, cpu);
	char* mssg = NULL;
	int result = NO_ERROR1;

	if (strcmp(won, PLAYER1_WIN) == 0)
	{
		result = sendGameResultMessage(SERVER_GAME_RESULTS, SERVER_NAME, cpu, move, getName(), *param->sd);

		addToLeaderInstanse(getName(), 1, 0);
		addToLeaderInstanse(SERVER_NAME, 0, 1);
	}
	else if (strcmp(won, PLAYER2_WIN) == 0)
	{
		result = sendGameResultMessage(SERVER_GAME_RESULTS, SERVER_NAME, cpu, move, SERVER_NAME, *param->sd);

		addToLeaderInstanse(SERVER_NAME, 1, 0);
		addToLeaderInstanse(getName(), 0, 1);
	}
	else
	{
		result = sendGameResultMessage(SERVER_GAME_RESULTS, SERVER_NAME, cpu, move, DREW_IN_GAME, *param->sd);
	}


	result = sendGeneralMesseage(SERVER_GAME_OVER_MENU, *param->sd);

	return(NO_ERROR1);
}

int pharseClientReplay(SockParams * param)
{
	int result = sendGeneralMesseage(SERVER_PLAYER_MOVE_REQUEST, *param->sd);
	return(NO_ERROR1);
}

int pharseClientRefresh(SockParams * param)
{
	// Check update time.
	int isUpdate = getIsUpdated();

	if (isUpdate == TRUE_VAL)
	{
		return(pharseClientLeader(param, FALSE_VAL));
		setUpdateTime();
	}
	else
	{
		return(pharseClientLeader(param, TRUE_VAL));
	}
}

int pharseClientDisconnect(SockParams * param)
{
	printf("\nWe SUCKKKKKKKKKKKKKKKKKKKKKKK AND DONT DO ANYTHING TO DISCONNECT YOU PROPERLY\n");

	return(NO_ERROR1);
}

int pharseMessage(char* mssg, SockParams * param)
{
	int result = NO_ERROR1;

	if (mssg == NULL)
	{
		return(DISCONNECTED_FROM_SERVER);
	}

	char * header = strtok(mssg, PARAMETER_BEGIN_CHAR);
	char* params = strtok(NULL, PARAMETER_BEGIN_CHAR);

	if (strcmp(header, CLIENT_REQUEST) == 0)
	{
		result = pharseClientRequest(params, param);
	}
	else if (strcmp(header, CLIENT_MAIN_MENU) == 0)
	{
		result = pharseClientMainMenue(param);
	}
	else if (strcmp(header, CLIENT_CPU) == 0)
	{
		result = pharseClientCPU(param);
	}
	else if (strcmp(header, CLIENT_VERSUS) == 0)
	{
		result = pharseClientVS(param);
	}
	else if (strcmp(header, CLIENT_LEADERBOARD) == 0)
	{
		result = pharseClientLeader(param, FALSE_VAL);
	}
	else if (strcmp(header, CLIENT_PLAYER_MOVE) == 0)
	{
		result = pharseClientMove(params, param);
	}
	else if (strcmp(header, CLIENT_REPLAY) == 0)
	{
		result = pharseClientReplay(param);
	}
	else if (strcmp(header, CLIENT_REFRESH) == 0)
	{
		result = pharseClientRefresh(param);
	}
	else if (strcmp(header, CLIENT_DISCONNECT) == 0)
	{
		result = pharseClientDisconnect(param);
	}
	else
	{
		printf(UNSUPPURTED_MESSAGE);
	}
	return (result);
}
#pragma endregion