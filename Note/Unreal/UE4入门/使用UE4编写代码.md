# Unreal初学

在Unity中我们会编写组件，而Unreal中不仅仅可以编写组件还可以为Actor编写代码。

```C++
UCLASS()
class AMyActor : public AActor
{
    GENERATED_BODY()

    // Called at start of game.
    void BeginPlay();

    // Called when destroyed.
    void EndPlay(const EEndPlayReason::Type EndPlayReason);

    // Called every frame to update this actor.
    void Tick(float DeltaSeconds);
};
```

