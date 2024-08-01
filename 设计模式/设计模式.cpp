#include<iostream>
#include<queue>
#include<chrono>
#include<list>
using namespace std;

//抽象水果
class AbstractFruit {
public:
	virtual void ShowName() = 0;
};
//苹果
class Apple :public AbstractFruit {
public:
	virtual void ShowName() {
		cout << "我是苹果!" << endl;
	}
};
//香蕉
class Banana :public AbstractFruit {
public:
	virtual void ShowName() {
		cout << "我是香蕉!" << endl;
	}
};
//鸭梨
class Pear :public AbstractFruit {
public:
	virtual void ShowName() {
		cout << "我是鸭梨!" << endl;
	}
};
//水果工厂
class FruitFactor {
public:
	static AbstractFruit* CreateFruit(string flag) {
		if (flag == "apple") {
			return new Apple;
		}
		else if (flag == "banana") {
			return new Banana;
		}
		else if (flag == "pear") {
			return new Pear;
		}
		else {
			return NULL;
		}
	}
};
void test01() {
	FruitFactor* factory = new FruitFactor;
	AbstractFruit* fruit = factory->CreateFruit("apple");
	fruit->ShowName();
	delete fruit;

	fruit = factory->CreateFruit("banana");
	fruit->ShowName();
	delete fruit;

	fruit = factory->CreateFruit("pear");
	fruit->ShowName();
	delete fruit;

	fruit = NULL;
	delete factory;
	factory = NULL;
}



//水果类和前面相同
//抽象工厂
class AbstractFruitFactory {
public:
	virtual AbstractFruit* CreateFruit() = 0;
};

//苹果工厂
class AppleFactory :public AbstractFruitFactory {
public:
	virtual AbstractFruit* CreateFruit() {
		return new Apple;
	}
};
//香蕉工厂
class BananaFactory :public AbstractFruitFactory {
public:
	virtual AbstractFruit* CreateFruit() {
		return new Banana;
	}
};
//鸭梨工厂
class PearFactory :public AbstractFruitFactory {
public:
	virtual AbstractFruit* CreateFruit() {
		return new Pear;
	}
};

void test02() {
	AbstractFruitFactory* factory = nullptr;
	AbstractFruit* fruit = nullptr;

	//创建对应子工厂
	factory = new AppleFactory();
	fruit = factory->CreateFruit();
	fruit->ShowName();

	delete factory;
	delete fruit;
	//创建对应子工厂
	factory = new BananaFactory();
	fruit = factory->CreateFruit();
	fruit->ShowName();

	delete factory;
	delete fruit;
}

//抽象系统类
class AbstractOperationSystem {
public:
	virtual void welcome() = 0;
};
//抽象文件系统类
class AbstractFileSystem {
public:
	virtual void FileSys() = 0;
};
//抽象CPU类
class AbstractCpu {
public:
	virtual void ShowType() = 0;
};
//上述三个抽象类构成了产品族
class WindowsOperationSystem : public AbstractOperationSystem {
public:
	virtual void welcome() override {
		std::cout
			<< "welcome use Windows system" << std::endl;
	};
};
class LinuxOperationSystem : public AbstractOperationSystem {
public:
	virtual void welcome() override {
		std::cout
			<< "welcome use Linux system" << std::endl;
	};
};
class MacOperationSystem : public AbstractOperationSystem {
public:
	virtual void welcome() override {
		std::cout
			<< "welcome use Mac system" << std::endl;
	};
};
//上面三个类构成了操作系统抽象类的产品等级
class ELFFileSystem :public AbstractFileSystem {
public:
	virtual void FileSys() {
		std::cout << "this is ELF fileSys" << std::endl;
	};
};
class PEFileSystem :public AbstractFileSystem {
public:
	virtual void FileSys() override {
		std::cout << "this is PE fileSys" << std::endl;
	};
};
class MAC_0FileSystem :public AbstractFileSystem {
public:
	virtual void FileSys() override  {
		std::cout << "this is MAC_0 fileSys" << std::endl;
	};
};
//上面三个类构成了文件系统抽象类的产品等级
class InterCPU :public AbstractCpu {
public:
	virtual void ShowType() override  {
		std::cout << "this is Inter CPU" << std::endl;
	};
};
class AMDCPU :public AbstractCpu {
public:
	virtual void ShowType() override {
		std::cout << "this is AMD CPU" << std::endl;
	};
};
//上面两个类构成了文件系统抽象类的产品等级

//抽象工厂 实现了三种产品的生产工厂
class GUIFactory {
public:
	virtual std::unique_ptr<AbstractOperationSystem> LoadOperator() = 0;
	virtual std::unique_ptr<AbstractFileSystem> LoadFile() = 0;
	virtual std::unique_ptr<AbstractCpu> LoadCPU() = 0;
};
//具体工厂1
class WindowsFactory : public GUIFactory {
public:
	std::unique_ptr<AbstractOperationSystem> LoadOperator() override {
		return std::make_unique<WindowsOperationSystem>();
	}
	std::unique_ptr<AbstractFileSystem> LoadFile() override {
		return std::make_unique<PEFileSystem>();
	}
	std::unique_ptr<AbstractCpu> LoadCPU() override {
		return std::make_unique<InterCPU>();
	}
};
//具体工厂2
class LinuxFactory : public GUIFactory {
public:
	std::unique_ptr<AbstractOperationSystem> LoadOperator() override {
		return std::make_unique<LinuxOperationSystem>();
	}
	std::unique_ptr<AbstractFileSystem> LoadFile() override {
		return std::make_unique<ELFFileSystem>();
	}
	std::unique_ptr<AbstractCpu> LoadCPU() override {
		return std::make_unique<InterCPU>();
	}
};
void test03() {
	std::unique_ptr<GUIFactory> factory;

	factory = std::make_unique<WindowsFactory>();
	factory->LoadCPU()->ShowType();
	factory->LoadFile()->FileSys();
	factory->LoadOperator()->welcome();

	factory.reset();

	factory = std::make_unique<LinuxFactory>();
	factory->LoadCPU()->ShowType();
	factory->LoadFile()->FileSys();
	factory->LoadOperator()->welcome();
}

class AbstractCommonInterface {
public:
	virtual void run() = 0;

};

//我已经写好的系统
class Mysystem :public AbstractCommonInterface {
public:
	virtual void run() override {
		cout << "系统启动..." << endl;
	}
};

#include<mutex>
class MysystemProxy :public AbstractCommonInterface {
private:
	MysystemProxy(const string& username, const string& password) :mUsername(username), mPassword(password), pSystem(nullptr) {

	}
public:
	static std::shared_ptr<MysystemProxy> Instance(const string& username, const string& password)
	{
		//保证在多线程中单例安全
		static std::once_flag oc;
		std::call_once(oc, [&] {
			s_ptr = std::shared_ptr<MysystemProxy>(new MysystemProxy(username, password));
		});
		return s_ptr;
	}
	bool checkUsernameAndPassword() {
		if (mUsername == "root" && mPassword == "admin") {
			return true;
		}
		return false;
	}
	virtual void run() override {
		if (checkUsernameAndPassword()) {
			cout << "用户名和密码正确，验证通过..." << endl;
			if (this->pSystem == nullptr) {
				pSystem = new Mysystem();
			}
			this->pSystem->run();
		}
		else {
			cout << "用户名或密码错误，权限不足...." << endl;
		}
	}
	~MysystemProxy() {
		if (pSystem != nullptr) {
			delete pSystem;
		}
	}
private:
	Mysystem* pSystem;
	string mUsername;
	string mPassword;
	static std::shared_ptr<MysystemProxy>s_ptr;
};
std::shared_ptr<MysystemProxy>MysystemProxy::s_ptr = nullptr;
void test04() {
#if 0
	//这样不行。是个人都能启动
	Mysystem* system = new Mysystem;
	system->run();
#endif
	//调用代理模式
	std::shared_ptr<MysystemProxy>proxy = MysystemProxy::Instance("root","admin");
	proxy->run();
}

//咖啡的抽象类
class AbstractCoffee {
public:
	virtual std::string getDescription() const = 0;
	virtual double getCost()const = 0;
};
//咖啡的一个具体类
class SimpleCoffee :public AbstractCoffee{
public:
	virtual std::string getDescription() const override  {
		return "Simple Coffee";
	}
	virtual double getCost() const override {
		return 2.0;
	}
};
//装饰类的基类
class CoffeeDecorator : public AbstractCoffee {
protected:
	AbstractCoffee* decoratedCoffee;
public:
	CoffeeDecorator(AbstractCoffee* coffee) : decoratedCoffee(coffee) {}
	virtual ~CoffeeDecorator() {
		delete decoratedCoffee;
	}
};
//装饰类的一个具体 加牛奶的咖啡
class MilkDecorator : public CoffeeDecorator {
public:
	MilkDecorator(AbstractCoffee* coffee) : CoffeeDecorator(coffee) {}

	std::string getDescription() const override {
		return decoratedCoffee->getDescription() + ", With Milk";
	}

	double getCost() const override {
		return decoratedCoffee->getCost() + 0.5; // cost of milk
	}
};
//装饰类的一个具体 加糖的咖啡
class SugarDecorator : public CoffeeDecorator {
public:
	SugarDecorator(AbstractCoffee* coffee) : CoffeeDecorator(coffee) {}

	std::string getDescription() const override {
		return decoratedCoffee->getDescription() + ", With Sugar";
	}

	double getCost() const override {
		return decoratedCoffee->getCost() + 0.5; // cost of milk
	}
};


void test05() {
	AbstractCoffee* myCoffee = new SimpleCoffee();
	std::cout << myCoffee->getDescription() << " $" << myCoffee->getCost() << std::endl;

	myCoffee = new MilkDecorator(myCoffee);
	std::cout << myCoffee->getDescription() << " $" << myCoffee->getCost() << std::endl;

	myCoffee = new SugarDecorator(myCoffee);
	std::cout << myCoffee->getDescription() << " $" << myCoffee->getCost() << std::endl;

	delete myCoffee; // Don't forget to delete the dynamically allocated memory
}



//电视机
class Televison {
public:
	void On() {
		cout << "电视机打开..." << endl;
	}
	void Off() {
		cout << "电视机关闭" << endl;
	}
};
//灯
class Light {
public:
	void On() {
		cout << "灯打开..." << endl;
	}
	void Off() {
		cout << "灯关闭" << endl;
	}
};
//音箱
class Audio {
public:
	void On() {
		cout << "音箱打开..." << endl;
	}
	void Off() {
		cout << "音箱关闭" << endl;
	}
};
//麦克风
class Microphone {
public:
	void On() {
		cout << "麦克风打开..." << endl;
	}
	void Off() {
		cout << "麦克风关闭" << endl;
	}
};
//DVD
class DVDPlayer {
public:
	void On() {
		cout << "DVD播放器打开..." << endl;
	}
	void Off() {
		cout << "DVD播放器关闭" << endl;
	}
};
//游戏机
class Gamemachine {
public:
	void On() {
		cout << "游戏机打开..." << endl;
	}
	void Off() {
		cout << "游戏机关闭" << endl;
	}
};

//KTV模式
class KTVModel {
private:
	bool is_off;
public:
	KTVModel() {
		pTv = new Televison;
		pLight = new Light;
		pAudio = new Audio;
		pMicrophone = new Microphone;
		pDVD = new DVDPlayer;
		is_off = true;
	}
	void OnKtv() {
		if (!is_off) { return; }
		pTv->On();
		pLight->Off();
		pAudio->On();
		pMicrophone->On();
		pDVD->On();
		is_off = false;
	}
	void OffKtv() {
		if (is_off) { return; }
		pTv->Off();
		pLight->On();
		pAudio->Off();
		pMicrophone->Off();
		pDVD->Off();
		is_off = true;
	}
	~KTVModel() {
		if (pTv != nullptr) {
			delete pTv;
		}
		if (pLight != nullptr) {
			delete pLight;
		}
		if (pAudio != nullptr) {
			delete pAudio;
		}
		if (pMicrophone != nullptr) {
			delete pMicrophone;
		}
		if (pDVD != nullptr) {
			delete pDVD;
		}
	}
public:
	Televison* pTv;
	Light* pLight;
	Audio* pAudio;
	Microphone* pMicrophone;
	DVDPlayer* pDVD;
};
void test06() {
	KTVModel* client = new KTVModel;
	client->OnKtv();
	client->OffKtv();
	delete client;
}




// 旧的打印系统接口
class OldPrinter {
public:
	void printOldFormat(const string& oldText) {
		cout << "Old Printer: " << oldText << endl;
	}
};
class NewPrinter {
public:
	virtual void printNewFormat(const string& newText) = 0;
	virtual ~NewPrinter() = default;
};
// 新的打印系统实现
class NewPrinterImpl : public NewPrinter {
public:
	void printNewFormat(const string& newText) override {
		cout << "New Printer: " << newText << endl;
	}
};
// 适配器类
class PrinterAdapter : public NewPrinter {
private:
	OldPrinter* oldPrinter;
public:
	PrinterAdapter(OldPrinter* oldPrinter) : oldPrinter(oldPrinter) {}

	void printNewFormat(const string& newText) override {
		// 将新格式的文本转换为旧格式
		string oldText = "Adapted: " + newText;
		oldPrinter->printOldFormat(oldText);
	}
};
void test07() {
	OldPrinter* oldPrinter = new OldPrinter();
	NewPrinter* newPrinter = new NewPrinterImpl();

	// 通过适配器使用旧的打印系统打印新格式的文本
	NewPrinter* adapter = new PrinterAdapter(oldPrinter);

	string newText = "This is new format text.";

	cout << "Using New Printer directly:" << endl;
	newPrinter->printNewFormat(newText);

	cout << "Using Adapter with Old Printer:" << endl;
	adapter->printNewFormat(newText);

	// 释放内存
	delete oldPrinter;
	delete newPrinter;
	delete adapter;
}


class DrinkTemplate {
public:
	//注水
	virtual void Boildwater() = 0;
	//冲泡
	virtual void Brew() = 0;
	//倒入杯中
	virtual void PourInCup() = 0;
	//加辅助料
	virtual void AddSomething() = 0;

	//模板方法
	void Make() {
		Boildwater();
		Brew();
		PourInCup();
		AddSomething();
	}
};

//冲泡咖啡
class Coffee : public DrinkTemplate {
public:
	//注水
	virtual void Boildwater() {
		cout << "煮山泉水..." << endl;
	}
	//冲泡
	virtual void Brew() {
		cout << "冲泡咖啡..." << endl;
	}
	//倒入杯中
	virtual void PourInCup() {
		cout << "咖啡倒入杯中..." << endl;
	}
	//加辅助料
	virtual void AddSomething() {
		cout << "加糖，加牛奶，加点醋..." << endl;
	}
};

//冲泡咖啡
class Tea : public DrinkTemplate {
public:
	//注水
	virtual void Boildwater() {
		cout << "煮自来水..." << endl;
	}
	//冲泡
	virtual void Brew() {
		cout << "冲泡铁观音..." << endl;
	}
	//倒入杯中
	virtual void PourInCup() {
		cout << "茶水倒入杯中..." << endl;
	}
	//加辅助料
	virtual void AddSomething() {
		cout << "加糖，加柠檬，加生姜..." << endl;
	}
};

void test08() {
	DrinkTemplate* tea = new Tea;
	tea->Make();
	cout << "----------------" << endl;
	DrinkTemplate* coffee = new Coffee;
	coffee->Make();
	delete tea;
	delete coffee;
}





//协议处理类
class HandleClientProtocol {
public:
	//处理增加金币
	void AddMoney() {
		cout << "给玩家增加金币！" << endl;
	}
	//处理增加钻石
	void AddDiamod() {
		cout << "给玩家增加钻石！" << endl;
	}
	//处理玩家装备
	void AddEquipment() {
		cout << "给玩家穿装备！" << endl;
	}
	//处理玩家升级
	void AddLevel() {
		cout << "给玩家升级！" << endl;
	}
};

//命令接口
class AbstractCommand {
public:
	virtual void handle() = 0; //处理客户端请求的接口
};

//处理增加金币请求
class AddMoneyCommand :public AbstractCommand {
public:
	AddMoneyCommand(HandleClientProtocol* protocol) {
		this->pProtocol = protocol;
	}
	virtual void handle() {
		this->pProtocol->AddMoney();
	}
public:
	HandleClientProtocol* pProtocol;
};

//处理增加钻石的请求
class AddDimondCommnd :public AbstractCommand {
public:
	AddDimondCommnd(HandleClientProtocol* protocol) {
		this->pProtocol = protocol;
	}
	virtual void handle() {
		this->pProtocol->AddDiamod();
	}
public:
	HandleClientProtocol* pProtocol;
};

//处理玩家装备装备的请求
class AddEquipmentCommnd :public AbstractCommand {
public:
	AddEquipmentCommnd(HandleClientProtocol* protocol) {
		this->pProtocol = protocol;
	}
	virtual void handle() {
		this->pProtocol->AddEquipment();
	}
public:
	HandleClientProtocol* pProtocol;
};

//处理玩家升级的请求
class AddLevelCommand :public AbstractCommand {
public:
	AddLevelCommand(HandleClientProtocol* protocol) {
		this->pProtocol = protocol;
	}
	virtual void handle() {
		this->pProtocol->AddLevel();
	}
public:
	HandleClientProtocol* pProtocol;
};

//服务器程序
class Server {
public:
	void addRequest(AbstractCommand* command) {
		mCommands.push(command);
	}

	void startHandle() {
		while (!mCommands.empty()) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			AbstractCommand* command = mCommands.front();
			command->handle();
			mCommands.pop();
		}
	}
public:
	queue<AbstractCommand*> mCommands;
};

void test09(){
	HandleClientProtocol* protocol = new HandleClientProtocol;
	//客户端增加金币的请求
	AbstractCommand* addmoney = new AddMoneyCommand(protocol);
	//客户端增加钻石的请求
	AbstractCommand* adddimond = new AddDimondCommnd(protocol);
	//客户端穿装备的请求
	AbstractCommand* addequiment = new AddEquipmentCommnd(protocol);
	//客户端升级的请求
	AbstractCommand* addlevel = new AddLevelCommand(protocol);

	//将客户端请求加入到处理的队列中
	Server* server = new Server;
	server->addRequest(addmoney);
	server->addRequest(adddimond);
	server->addRequest(addequiment);
	server->addRequest(addlevel);

	//服务器开始处理请求
	server->startHandle();
}



//抽象武器 武器策略
class WeaponStrategy {
public:
	virtual void UseWeapon() = 0;
};

class Knife : public WeaponStrategy {
public:
	virtual void UseWeapon() {
		cout << "使用匕首!" << endl;
	}
};

class AK47 : public WeaponStrategy {
public:
	virtual void UseWeapon() {
		cout << "使用AK47!" << endl;
	}
};

class Character {
public:
	void setWeapon(WeaponStrategy* weapon) {
		this->pWeapon = weapon;
	}
	void ThrowWeapon() {
		this->pWeapon->UseWeapon();
	}
private:
	WeaponStrategy* pWeapon;
};

void test10() {
	//创建角色
	Character* character = new Character;

	//武器策略
	WeaponStrategy* knife = new Knife;
	WeaponStrategy* ak47 = new AK47;

	character->setWeapon(knife);
	character->ThrowWeapon();

	character->setWeapon(ak47);
	character->ThrowWeapon();

	delete ak47;
	delete knife;
	delete character;
}


//抽象的英雄
class AbstractHero {
public:
	virtual void Update() = 0;

};

class HeroA :public AbstractHero {
public:
	HeroA() {
		cout << "英雄A正在撸Boss..." << endl;
	}
	virtual void Update() {
		cout << "英雄A停止撸，待机状态..." << endl;
	}
};
class HeroB :public AbstractHero {
public:
	HeroB() {
		cout << "英雄B正在撸Boss..." << endl;
	}
	virtual void Update() {
		cout << "英雄B停止撸，待机状态..." << endl;
	}
};
class HeroC :public AbstractHero {
public:
	HeroC() {
		cout << "英雄C正在撸Boss..." << endl;
	}
	virtual void Update() {
		cout << "英雄C停止撸，待机状态..." << endl;
	}
};
class HeroD :public AbstractHero {
public:
	HeroD() {
		cout << "英雄D正在撸Boss..." << endl;
	}
	virtual void Update() {
		cout << "英雄D停止撸，待机状态..." << endl;
	}
};
class HeroE :public AbstractHero {
public:
	HeroE() {
		cout << "英雄E正在撸Boss..." << endl;
	}
	virtual void Update() {
		cout << "英雄E停止撸，待机状态..." << endl;
	}
};

//观察目标抽象
class AbstractBoss {
public:
	//添加观察者
	virtual void addHero(AbstractHero* hero) = 0;
	//删除观察者
	virtual void deleteHero(AbstractHero* hero) = 0;
	//通知所有观察者
	virtual void notify() = 0;
};

//具体的观察者 BOSSA
class BOSSA :public AbstractBoss {
public:
	//添加观察者
	virtual void addHero(AbstractHero* hero) {
		pHeroList.push_back(hero);
	}
	//删除观察者
	virtual void deleteHero(AbstractHero* hero) {
		pHeroList.remove(hero);
	}
	//通知所有观察者
	virtual void notify() {
		for (list<AbstractHero*>::iterator it = pHeroList.begin(); it != pHeroList.end(); it++) {
			(*it)->Update();
		}
	}
public:
	list<AbstractHero*> pHeroList;
};

void test11() {
	//创建观察者
	AbstractHero* heroA = new HeroA;
	AbstractHero* heroB = new HeroB;
	AbstractHero* heroC = new HeroC;
	AbstractHero* heroD = new HeroD;
	AbstractHero* heroE = new HeroE;

	//创建观测目标
	AbstractBoss* bossA = new BOSSA;
	bossA->addHero(heroA);
	bossA->addHero(heroB);
	bossA->addHero(heroC);
	bossA->addHero(heroD);
	bossA->addHero(heroE);

	//模拟控制技能发动 其他被观察者不能发动攻击
	bossA->notify();
}

int main() {
	//简单工厂模式
	//test01();
	//工厂方法模式
	//test02();
	//抽象工厂模式
	//test03();
	//代理模式
	//test04();
	//装饰模式
	//test05();
	//外观模式
	//test06();
	//适配器模式
	//test07();
	//模板方法模式
	//test08();
	//命令模式
	//test09();
	//策略模式
	//test10();
	//观察者模式
	test11();
	return 0;
}