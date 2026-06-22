# 03 · UE / GAS 实例对照

> 用 Aura / GAS 真实结构练习读图、画图。  
> 线型规则 → [01-类之间的关系.md](./01-类之间的关系.md)

---

## 1. GAS 核心结构总图

```text
                    Activate
OwnerActor - - - - - - - - - - - > UGameplayAbility
    ^                                    │
    │                                    │ ◇ ability
    │                                    v
    └ - - - - - - - - - - - - FGameplayAbilityActorInfo
                                        │
FGameplayAbilitySpec ◇──────────────────┘
    │ Ability*
    │ Level
    │ ActivationInfo
    │
    v
FGameplayAbilitySpecContainer ◆── TArray<FGameplayAbilitySpec> Items
    │
    v
UAbilitySystemComponent ◆── FGameplayAbilitySpecContainer ActivatableAbilities
```

---

## 2. 逐条解释

| 连线 | 关系 | 为什么 |
|------|------|--------|
| OwnerActor `- - ->` Ability | **依赖** | 激活时调用，非长期拥有 |
| Spec `◇──` Ability | **聚合** | Spec 引用 CDO，Ability 不随 Spec 销毁 |
| SpecContainer `◆──` Spec | **组合** | Container 拥有 Spec 数组 |
| ASC `◆──` SpecContainer | **组合** | ASC 销毁则 ActivatableAbilities 清空 |
| AuraAbility `- - - ▷` GameplayAbility | **继承** | 子类 is-a 父类 |

---

## 3. Aura 项目代码对照

### GiveAbility 流程

```cpp
// AuraAbilitySystemComponent.cpp
FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(CharacterAbility, 1);
AbilitySpec.DynamicAbilityTags.AddTag(AuraAbility->StarupInputTag);
GiveAbility(AbilitySpec);
```

- `CharacterAbility`：`TSubclassOf<UGameplayAbility>` → 指向 **CDO**
- `GiveAbility` 在 ASC 的 **ActivatableAbilities** 里 **新增一个 Spec**（组合）
- Spec 里保存 Ability 指针 → **聚合**

### ApplyEffect 流程

```text
AAuraEffectActor - - - - -> UAbilitySystemComponent
                              │
                              │ ApplyGameplayEffectSpecToSelf
                              v
                         FActiveGameplayEffect
                              │
                              ◇── UGameplayEffect* (Def/CDO)
```

- EffectActor **依赖** ASC 来 Apply
- ActiveEffect **聚合** GE 资产定义
- ASC **组合** ActiveGameplayEffects 容器（同理 Spec）

---

## 4. 类继承链示例

```text
AAuraCharacterBase ─────▷ ACharacter ─────▷ APawn ─────▷ AActor
UAuraAbilitySystemComponent ─────▷ UAbilitySystemComponent ─────▷ UActorComponent
UAuraGameplayAbility ─────▷ UGameplayAbility
UAuraAttributeSet ─────▷ UAttributeSet
```

**画法**：实线 + 空心三角，三角指向父类。

---

## 5. 什么时候画关联而不是聚合？

| 场景 | 建议 |
|------|------|
| `AAuraCharacter` 持有 `AbilitySystemComponent` 指针 | **关联** 或 **组合**（若强调同生命周期） |
| `PlayerController` 引用 `Pawn` | **关联** |
| Spec 引用 Ability CDO | **聚合** |
| ASC 内 `TArray<FGameplayAbilitySpec>` | **组合** |
| 一次 `TryActivateAbility` | **依赖** |

日常架构图：**菱形 = 容器/包含；实线箭头 = 普通引用** 即可，不必过度纠结。

---

## 6. 与 GA / GE Tag 的关系（读图扩展）

类图 **不直接画 Tag**，但可标注行为：

```text
UGameplayAbility
  ActivationOwnedTags     → 激活期间赋予 Tag（GA 侧）
UGameplayEffect (Component)
  TargetTags Component    → 效果期间赋予 Tag（GE 侧，UE 5.3+）
```

这是 **运行时行为**，类图上通常用 Note 说明，而不是专门的关系线型。

---

## 7. 复习检查

- [ ] 能独立画出 GAS Spec / ASC / Ability 三者关系吗？  
- [ ] GiveAbility 对应组合还是聚合？  
- [ ] Activate 对应依赖还是关联？  
- [ ] Aura 四条继承链能画对吗？
