#pragma once

class CProtocolBuffer;

void PacketProc_CreateMyCharacter(CProtocolBuffer *cpPacket);

void PacketProc_CreateOtherCharacter(CProtocolBuffer *cpPacket);

void PacketProc_CreateMyCharacter(CProtocolBuffer *cpPacket);

void PacketProc_DeleteCharacter(CProtocolBuffer *cpPacket);

void PacketProc_MoveStart(CProtocolBuffer *cpPacket);

void PacketProc_MoveStop(CProtocolBuffer *cpPacket);

void PacketProc_Attack1(CProtocolBuffer *cpPacket);

void PacketProc_Attack2(CProtocolBuffer *cpPacket);

void PacketProc_Attack3(CProtocolBuffer *cpPacket);

void PacketProc_Damage(CProtocolBuffer *cpPacket);
