#include "CollisionManager.h"
#include "..\GameObject\Skills\Skill.h"

CollisionManager::CollisionManager()
{
}

CollisionManager::~CollisionManager()
{
}

void CollisionManager::CheckCollision(UnitManager* unit_manager, SkillEffectManager* skillEffect_manager, bool bossCaptureScene)
{
	if (!bossCaptureScene)
	{
		if (unit_manager)
		{
			Unit* player = unit_manager->GetPlayer();
			Unit* enemyCastle = unit_manager->GetEnemyCastle();
			Unit* enemyBoss = unit_manager->GetEnemyBoss();
			// For each Enemy Units,
			for (Unit* enemy : *unit_manager->GetEnemyUnitsList())
			{
				if (enemy->IfActive())
				{
					if (player->IfActive())
					{
						Hero* playerHero = (Hero*)player;
						if (playerHero->IfMovingRight())
							// Player check collision with Enemy Unit
							player->CheckTargetWhetherToAttack(enemy);
						else
							// Enemy remove target: Player
							enemy->RemoveTarget(player);
						if (enemy->GetState() != Unit::KNOCKED_UP)
							// Enemy Unit check collision with Player
							enemy->CheckTargetWhetherToAttack(player);
					}
					// For each Player Units,
					for (Unit* unit : *unit_manager->GetPlayerUnitsList())
					{
						if (unit->IfActive())
						{
							// Player Unit check collision with Enemy Unit
							unit->CheckTargetWhetherToAttack(enemy);
							if (enemy->GetState() != Unit::KNOCKED_UP)
								// Enemy Unit check collision with Player Unit
								enemy->CheckTargetWhetherToAttack(unit);

						}
					}
				}
			}

			// Castle: Enemy
			if (enemyCastle)
			{
				if (enemyCastle->IfActive())
				{
					if (player->IfActive())
					{
						Hero* playerHero = (Hero*)player;
						if (playerHero->IfMovingRight())
							// Player check collision with Enemy Castle
							player->CheckTargetWhetherToAttack(enemyCastle);
					}
					// For each Player Units,
					for (Unit* unit : *unit_manager->GetPlayerUnitsList())
					{
						if (unit->IfActive())
						{
							// Player Unit check collision with Enemy Castle
							unit->CheckTargetWhetherToAttack(enemyCastle);
						}
					}
				}
			}

			// Boss: Enemy
			if (enemyBoss->IfActive())
			{
				if (player->IfActive())
				{
					Hero* playerHero = (Hero*)player;
					if (playerHero->IfMovingRight())
						// Player check collision with Enemy Boss
						player->CheckTargetWhetherToAttack(enemyBoss);
					else
						// Enemy Boss remove target: Player
						enemyBoss->RemoveTarget(player);

					// Enemy Boss check collision with Player
					enemyBoss->CheckTargetWhetherToAttack(player);
				}
				// For each Player Units,
				for (Unit* unit : *unit_manager->GetPlayerUnitsList())
				{
					if (unit->IfActive())
					{
						// Player Unit check collision with Enemy Boss
						unit->CheckTargetWhetherToAttack(enemyBoss, false);
						// Enemy Boss check collision with Player Unit
						enemyBoss->CheckTargetWhetherToAttack(unit);
					}
				}
			}

			if (skillEffect_manager)
			{
				// For each Player Units,
				for (SkillEffect* skillEffect : *skillEffect_manager->GetSkillEffectList())
				{
					if (skillEffect->IfActive())
					{
						auto skillEffectSprite = (Sprite*)skillEffect->GetSprite();
						auto seqeunce = (Sequence*)skillEffectSprite->getActionByTag(SkillEffect::ANIMATION);
						if (seqeunce)
						{
							if (seqeunce->getElapsed() >= skillEffect->GetTimeToDamage())
							{
								if (skillEffectSprite->getTag() != Skill::ICE_SPEAR &&
									skillEffectSprite->getTag() != Skill::FIREBALL)
									skillEffect->SetTimeToDamage(seqeunce->getDuration() + 1);
								Rect skillEffectRect(	skillEffect->getPositionX() - skillEffectSprite->getBoundingBox().size.width *  0.5f, skillEffect->getPositionY(),
														skillEffectSprite->getBoundingBox().size.width, skillEffectSprite->getBoundingBox().size.height);
								
								if (skillEffectSprite->getTag() == Skill::HURRICANE)
								{
									// For each Enemy Units,
									for (Unit* enemy : *unit_manager->GetEnemyUnitsList())
									{
										if (enemy->IfActive())
										{
											Rect enemyRect(	enemy->getPositionX(), enemy->getPositionY(),
															enemy->GetSprite()->getBoundingBox().size.width, enemy->GetSprite()->getBoundingBox().size.height);
											// Enemy check collision with skillEffect
											if (skillEffectRect.intersectsRect(enemyRect) && enemy->GetState() != Unit::KNOCKED_UP)
											{
												enemy->KnockUp(seqeunce->getDuration(), skillEffectSprite->getScaleY() * skillEffectSprite->getContentSize().width, skillEffect->GetDamage());
												break;
											}
										}
									}
								}
								else
								{
									// For each Enemy Units,
									for (Unit* enemy : *unit_manager->GetEnemyUnitsList())
									{
										if (enemy->IfActive())
										{
											Rect enemyRect(	enemy->getPositionX(), enemy->getPositionY(),
															enemy->GetSprite()->getBoundingBox().size.width, enemy->GetSprite()->getBoundingBox().size.height);
											// Enemy check collision with skillEffect
											if (skillEffectRect.intersectsRect(enemyRect))
											{
												enemy->MinusHeallth(skillEffect->GetDamage());

												if (enemy->IfActive())
												{
													switch (skillEffectSprite->getTag())
													{
														case Skill::FIREBALL:
														{
															enemy->Burn(10.f, 5, 30.f);
															break;
														}
														case Skill::ICE_SPEAR:
														{
															enemy->Freeze(5.f, 20.f);
															break;
														}
														case Skill::LIGHTNING:
														{
															enemy->ElectricShock(8.f, 15, 10.f);
															break;
														}
													}
												}

												if (skillEffectSprite->getTag() == Skill::ICE_SPEAR ||
													skillEffectSprite->getTag() == Skill::FIREBALL)
												{
													skillEffect->Stop();
													break;
												}
											}
										}
									}
									if ((skillEffectSprite->getTag() == Skill::ICE_SPEAR ||
										skillEffectSprite->getTag() == Skill::FIREBALL) &&
										!skillEffect->IfActive())
										break;
								}
								
								// Enemy Boss
								if (enemyBoss)
								{
									if (enemyBoss->IfActive())
									{
										Rect enemyBossRect(enemyBoss->getPositionX(), enemyBoss->getPositionY(),
											enemyBoss->GetSprite()->getBoundingBox().size.width, enemyBoss->GetSprite()->getBoundingBox().size.height);
										// Enemy boss check collision with skillEffect
										if (skillEffectRect.intersectsRect(enemyBossRect))
										{
											enemyBoss->MinusHeallth(skillEffect->GetDamage());
											if (enemyBoss->IfActive())
											{
												switch (skillEffectSprite->getTag())
												{
													case Skill::LIGHTNING:
													{
														enemyBoss->ElectricShock(8.f, 15, 10.f);
														break;
													}
												}
											}

											if (skillEffectSprite->getTag() == Skill::ICE_SPEAR ||
												skillEffectSprite->getTag() == Skill::FIREBALL)
											{
												skillEffect->Stop();
												break;
											}
										}
									}
								}
								// Castle: Enemy
								if (enemyCastle)
								{
									if (enemyCastle->IfActive())
									{
										Rect enemyCastleRect(enemyCastle->getPositionX(), enemyCastle->getPositionY(),
											enemyCastle->GetSprite()->getBoundingBox().size.width, enemyCastle->GetSprite()->getBoundingBox().size.height);
										// Enemy boss check collision with skillEffect
										if (skillEffectRect.intersectsRect(enemyCastleRect))
										{
											enemyCastle->MinusHeallth(skillEffect->GetDamage());
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	else
	{
		if (unit_manager)
		{
			Unit* player = unit_manager->GetPlayer();
			Unit* enemyBoss = unit_manager->GetEnemyBoss();
			// For each Enemy Units,
			for (Unit* enemy : *unit_manager->GetEnemyUnitsList())
			{
				if (enemy->IfActive())
				{
					// For each Player Units,
					for (Unit* unit : *unit_manager->GetPlayerUnitsList())
					{
						if (unit->IfActive())
						{
							// Enemy remove target: Player Unit
							enemy->CheckTargetWhetherToRemove(unit, false);
							// Enemy check collision with Player Unit
							enemy->CheckTargetWhetherToAttack(unit, false);

							// Player Unit remove target: Enemy
							unit->CheckTargetWhetherToRemove(enemy, false);
							// Player Unit check collision with Enemy
							unit->CheckTargetWhetherToAttack(enemy, false);
						}
					}
					if (player->IfActive())
					{
						// Player remove target: Enemy
						player->CheckTargetWhetherToRemove(enemy, false);
						// Player Unit check collision with Enemy
						player->CheckTargetWhetherToAttack(enemy, false);

						// Enemy remove target: Player
						enemy->CheckTargetWhetherToRemove(player, false);
						// Enemy Unit check collision with Player
						enemy->CheckTargetWhetherToAttack(player, false);
					}
				}
			}

			// Boss: Enemy
			if (enemyBoss->IfActive())
			{
				// For each Player Units,
				for (Unit* unit : *unit_manager->GetPlayerUnitsList())
				{
					if (unit->IfActive())
					{
						// Enemy Boss remove target: Player
						enemyBoss->CheckTargetWhetherToRemove(unit, false);
						// Enemy Boss check collision with Player
						enemyBoss->CheckTargetWhetherToAttack(unit, false);

						// Player Unit remove target: Enemy Boss
						unit->CheckTargetWhetherToRemove(enemyBoss, false);
						// Player Unit check collision with Enemy Boss
						unit->CheckTargetWhetherToAttack(enemyBoss, false);
					}
				}
				if (player->IfActive())
				{
					// Player remove target: Enemy Boss
					player->CheckTargetWhetherToRemove(enemyBoss, false);
					// Player check collision with Enemy Boss
					player->CheckTargetWhetherToAttack(enemyBoss, false);

					// Enemy Boss remove target: Player
					enemyBoss->CheckTargetWhetherToRemove(player, false);
					// Enemy Boss check collision with Player
					enemyBoss->CheckTargetWhetherToAttack(player, false);
				}
			}
		}
	}
}