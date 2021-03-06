//
//  TRHero.cpp
//  TombRaider
//
//  Created by 張正昊 on 12/2/2016.
//  Copyright © 2016 hahaschool. All rights reserved.
//

#include "TRHero.hpp"

TRHero::TRHero(){
    
}

#pragma mark - 属性

void TRHero::setMaxHP(int nmxhp){
    mxhp = nmxhp;
}

void TRHero::setHP(int nhp){
    hp = nhp;
}
void TRHero::setVelocity(int nvel){
    vel = nvel;
}

void TRHero::setArmour(int narm){
    arm = narm;
}

void TRHero::setDamage(int ndmg){
    dmg = ndmg;
}

int TRHero::getMaxHP(){
    return mxhp;
}

int TRHero::getHP(){
    return hp;
}

int TRHero::getVelocity(){
    return vel;
}

int TRHero::getArmour(){
    return arm;
}

int TRHero::getDamage(){
    return dmg;
}

#pragma mark - 死亡

void TRHero::setDeathClip(SDL_Rect clipRect,TRDirection dir){
    deathRect[dir] = clipRect;
}

void TRHero::goDie(){
    setStaticAnimated(false);
    freeze();
    setStaticClip(deathRect[TRDirectionUp], TRDirectionUp);
    setStaticClip(deathRect[TRDirectionDown], TRDirectionDown);
    setStaticClip(deathRect[TRDirectionLeft], TRDirectionLeft);
    setStaticClip(deathRect[TRDirectionRight], TRDirectionRight);
    alive = false;
}

#pragma mark - 被弹

void TRHero::beAttacked(int dmg){
    if(arm < dmg){
        hp -= dmg - arm;
    }
    if(hp <= 0){
        hp = 0;
        goDie();
    }
}

#pragma mark - 攻击

bool TRHero::performAttack(){
    if(!flgdebuff_freeze && atk_cd_rem <= 0){
        if(isAttackingAnimated()){
            linkAttackingAnimator();
            lock(atk_cd);
        }
        atk_cd_rem = atk_cd;
        attacking = true;
        return true;
    }
    return false;
}

void TRHero::endAttack(){
    linkStandingAnimator();
    unlock();
    attacking = false;
}

void TRHero::attack(TREnemy *obj){
    obj->beAttacked(dmg);
}

void TRHero::setAttackCD(int ncd){
    atk_cd = ncd;
}

int TRHero::getAttackCD(){
    return atk_cd;
}

#pragma mark - 不良状态
#pragma mark 减速
//不良状态
void TRHero::debuff_Slow(int interval,int dmg){
    debuff_Slow_clear();
    debuff_slow_rem = interval;
    debuff_slow_dmg = dmg;
    vel -= dmg;
    flgdebuff_slow = true;
}

void TRHero::debuff_Slow_clear(){
    flgdebuff_slow = false;
    vel += debuff_slow_dmg;
    debuff_slow_dmg = 0;
}

bool TRHero::is_debuff_Slow(){
    return flgdebuff_slow;
}

int TRHero::dmg_debuff_Slow(){
    return debuff_slow_dmg;
}

int TRHero::rem_debuff_Slow(){
    return debuff_slow_rem;
}

#pragma mark 出血

void TRHero::debuff_Bleed(int interval,int dmg){
    debuff_Bleed_clear();
    debuff_bleed_rem = interval;
    debuff_bleed_dmg = dmg;
    flgdebuff_bleed = true;
}

void TRHero::debuff_Bleed_clear(){
    flgdebuff_bleed = false;
}

bool TRHero::is_debuff_Bleed(){
    return flgdebuff_bleed;
}

int TRHero::dmg_debuff_Bleed(){
    return debuff_bleed_dmg;
}

int TRHero::rem_debuff_Bleed(){
    return debuff_bleed_rem;
}

#pragma mark 冻结（无法移动）

void TRHero::debuff_Freeze(int interval){
    debuff_freeze_rem = interval;
    flgdebuff_freeze = true;
    freeze();
}

void TRHero::debuff_Freeze_clear(){
    flgdebuff_freeze = false;
    defreeze();
}

bool TRHero::is_debuff_Freeze(){
    return flgdebuff_freeze;
}

int TRHero::rem_debuff_Freeze(){
    return debuff_freeze_rem;
}

#pragma mark 缴械（无法远程攻击）

void TRHero::debuff_Nogun(int interval){
    debuff_nogun_rem = interval;
    flgdebuff_nogun = true;
}

void TRHero::debuff_Nogun_clear(){
    flgdebuff_nogun = false;
}

bool TRHero::is_debuff_Nogun(){
    return flgdebuff_nogun;
}

int TRHero::rem_debuff_Nogun(){
    return debuff_nogun_rem;
}

#pragma mark 消除全部不良状态

void TRHero::debuff_clear(){
    debuff_Slow_clear();
    debuff_Nogun_clear();
    debuff_Bleed_clear();
    debuff_Freeze_clear();
}

#pragma mark - 远程攻击
TRBullet* TRHero::fire(){
    if(bullet_cd_rem <= 0 && !flgdebuff_nogun){
        lock(fire_cd);
    }
    
    
    return (TRBullet*)NULL;
}

void TRHero::setBulletDamage(int ndmg){
    bullet_dmg = ndmg;
}

int TRHero::getBulletDamage(){
    return bullet_dmg;
}

void TRHero::setBulletSpeed(int nvel){
    bullet_vel = nvel;
}

int TRHero::getBulletSpeed(){
    return bullet_vel;
}

#pragma mark - 移动
void TRHero::startMoveUp(){
    linkWalkingAnimator();
    startMoving();
    setDirection(TRDirectionUp);
    setVelX(0);
    setVelY(-vel);
}

void TRHero::startMoveLeft(){
    linkWalkingAnimator();
    startMoving();
    setDirection(TRDirectionLeft);
    setVelX(-vel);
    setVelY(0);
}

void TRHero::startMoveDown(){
    linkWalkingAnimator();
    startMoving();
    setDirection(TRDirectionDown);
    setVelX(0);
    setVelY(vel);
}

void TRHero::startMoveRight(){
    linkWalkingAnimator();
    startMoving();
    setDirection(TRDirectionRight);
    setVelX(vel);
    setVelY(0);
}

void TRHero::endMoving(){
    setVelXY(0);
    TRSprite::endMoving();
}

void TRHero::move(){
    if (isMoving()) {
        TRSprite::move();
    }
}

#pragma mark - 渲染
void TRHero::render(){
    if(anilock){
        if(anilockrem-- <= 0){
            endAttack();
        }
    }
    atk_cd_rem--,bullet_cd_rem--;
    if(is_debuff_Bleed()){
        debuff_bleed_rem--;
        setHP(getHP() - debuff_bleed_dmg);
        if (debuff_bleed_rem <= 0) {
            debuff_Bleed_clear();
        }
    }
    if(is_debuff_Freeze()){
        freeze();
        debuff_freeze_rem--;
        if(debuff_freeze_rem <= 0){
            debuff_Freeze_clear();
        }
    }
    if(is_debuff_Nogun()){
        debuff_nogun_rem--;
        if (debuff_nogun_rem <= 0) {
            debuff_Nogun_clear();
        }
    }
    if(is_debuff_Slow()){
        debuff_slow_rem--;
        if(debuff_slow_rem <= 0){
            debuff_Slow_clear();
        }
    }
    move();
    TRSprite::render();
}

#pragma mark - 治疗
void TRHero::heal(int det){
    setHP(std::max(getMaxHP(),getHP()+det));
}

#pragma mark - 动画
#pragma mark 站立
void TRHero::setStandingAnimator(TRAnimator *ani, TRDirection dir){
    standingAnimator[dir] = ani;
}

void TRHero::linkStandingAnimator(){
    if(isStandingAnimated()){
        setStaticAnimated(true);
        linkStaticAnimator(standingAnimator[TRDirectionUp], TRDirectionUp);
        linkStaticAnimator(standingAnimator[TRDirectionLeft], TRDirectionLeft);
        linkStaticAnimator(standingAnimator[TRDirectionRight], TRDirectionRight);
        linkStaticAnimator(standingAnimator[TRDirectionDown], TRDirectionDown);
    }else{
        setStaticAnimated(false);
    }
}

void TRHero::setStandingAnimated(bool flg){
    standingAnimated = flg;
}

bool TRHero::isStandingAnimated(){
    return standingAnimated;
}
#pragma mark 走动
void TRHero::setWalkingAnimator(TRAnimator *ani, TRDirection dir){
    walkingAnimator[dir] = ani;
}

void TRHero::linkWalkingAnimator(){
    if(isWalkingAnimated()){
        setMovingAnimated(true);
        linkMovingAnimator(walkingAnimator[TRDirectionUp], TRDirectionUp);
        linkMovingAnimator(walkingAnimator[TRDirectionLeft], TRDirectionLeft);
        linkMovingAnimator(walkingAnimator[TRDirectionRight], TRDirectionRight);
        linkMovingAnimator(walkingAnimator[TRDirectionDown], TRDirectionDown);
    }else{
        setMovingAnimated(false);
    }
}

void TRHero::setWalkingAnimated(bool flg){
    walkingAnimated = flg;
}

bool TRHero::isWalkingAnimated(){
    return walkingAnimated;
}
#pragma mark 近身攻击
void TRHero::setAttackingAnimator(TRAnimator *ani, TRDirection dir){
    attackingAnimator[dir] = ani;
}

void TRHero::linkAttackingAnimator(){
    if(isAttackingAnimated()){
        setStaticAnimated(true);
        linkStaticAnimator(attackingAnimator[TRDirectionUp], TRDirectionUp);
        linkStaticAnimator(attackingAnimator[TRDirectionLeft], TRDirectionLeft);
        linkStaticAnimator(attackingAnimator[TRDirectionRight], TRDirectionRight);
        linkStaticAnimator(attackingAnimator[TRDirectionDown], TRDirectionDown);
    }else{
        setStaticAnimated(false);
    }
}

void TRHero::setAttackingAnimated(bool flg){
    attackingAnimated = flg;
}

bool TRHero::isAttackingAnimated(){
    return attackingAnimated;
}
#pragma mark 远程攻击
void TRHero::setFiringAnimator(TRAnimator *ani, TRDirection dir){
    firingAnimator[dir] = ani;
}

void TRHero::linkFiringAnimator(){
    if(isFiringAnimated()){
        setStaticAnimated(true);
        linkStaticAnimator(firingAnimator[TRDirectionUp], TRDirectionUp);
        linkStaticAnimator(firingAnimator[TRDirectionLeft], TRDirectionLeft);
        linkStaticAnimator(firingAnimator[TRDirectionRight], TRDirectionRight);
        linkStaticAnimator(firingAnimator[TRDirectionDown], TRDirectionDown);
    }else{
        setStaticAnimated(false);
    }
}

void TRHero::setFiringAnimated(bool flg){
    firingAnimated = flg;
}

bool TRHero::isFiringAnimated(){
    return firingAnimated;
}

#pragma mark - Animator Lockdown (PRIVATE)
void TRHero::lock(int interval){
    freeze();
    anilockrem = interval;
    anilock = true;
}
void TRHero::unlock(){
    defreeze();
    anilock = false;
    anilockrem = 0;
}