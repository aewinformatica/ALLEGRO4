import paintown

class MyCharacter(paintown.Character):
    def __init__(self, character, engine):
        paintown.Character.__init__(self, character)
        self.engine = engine
        self.add = False

    def didCollide(self, him):
        if self.add == False:
            self.engine.addCharacter("chars/joe/joe.txt", "Joey", 0, 100, 20, 20)
            self.add = True
        if self.engine.findObject(self.getId()) != self:
            print "Find object failed!"
        else:
            print "Found self, ok"

    def tick(self):
        # self.setHealth(100)
        pass

class MyPlayer(paintown.Player):
    def __init__(self, player):
        paintown.Player.__init__(self, player)

    def tick(self):
        self.increaseScore(1)

class MyEngine(paintown.Engine):
    def __init__(self):
        print "Hello from my own engine"
        paintown.Engine.__init__(self)

    def createWorld(self, world):
        paintown.Engine.createWorld(self, world)
        self.cacheCharacter("chars/joe/joe.txt")
        print "MyEngine: level length is " + str(self.levelLength())

    def createPlayer(self, player):
        return MyPlayer(player)

    def createCharacter(self, character):
        return MyCharacter(character, self)

    def tick(self):
        for i in self.getPlayers():
            print "Player id %d" % i.getId()
        for i in self.getEnemies():
            print "Enemy id %d" % i.getId()
        if False:
            print "World tick"
            for i in self.getObjects():
                print "Found object %d" % i.getId()
            print "World end tick"

paintown.register(MyEngine())

if False:
    import threading
    class MyThread(threading.Thread):
        def run(self):
            import time
            while True:
                print "Hi %d" % time.time()
                time.sleep(0.5)

    MyThread().start()
