#pragma once
void CreateMyCharactor(CPacket* cpPacket);
void CreateOtherCharactor(CPacket* cpPacket);
void DeleteCharacter(CPacket* cpPacket);
void CharacterMoveStart(CPacket* cpPacket);
void CharacterMoveStop(CPacket* cpPacket);
void CharacterAttack1(CPacket* cpPacket);
void CharacterAttack2(CPacket* cpPacket);
void CharacterAttack3(CPacket* cpPacket);

void CharacteDamage(CPacket* cpPacket);


void SendPacket(CPacket* cpPacket);
void SendPacket(stHEADER* pHeader, char* pPacket);
void WriteEvent();