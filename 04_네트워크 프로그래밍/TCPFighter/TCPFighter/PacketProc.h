#pragma once

void PacketProc_CreateMyCharacter(char *pPacket);

void PacketProc_CreateOtherCharacter(char *pPacket);

void PacketProc_CreateMyCharacter(char *pPacket);

void PacketProc_DeleteCharacter(char *pPacket);

void PacketProc_MoveStart(char *pPacket);

void PacketProc_MoveStop(char *pPacket);

void PacketProc_Attack1(char *pPacket);

void PacketProc_Attack2(char *pPacket);

void PacketProc_Attack3(char *pPacket);

void PacketProc_Damage(char *pPacket);
