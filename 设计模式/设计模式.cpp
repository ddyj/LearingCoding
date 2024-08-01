#include<iostream>
#include<queue>
#include<chrono>
#include<list>
using namespace std;

//����ˮ��
class AbstractFruit {
public:
	virtual void ShowName() = 0;
};
//ƻ��
class Apple :public AbstractFruit {
public:
	virtual void ShowName() {
		cout << "����ƻ��!" << endl;
	}
};
//�㽶
class Banana :public AbstractFruit {
public:
	virtual void ShowName() {
		cout << "�����㽶!" << endl;
	}
};
//Ѽ��
class Pear :public AbstractFruit {
public:
	virtual void ShowName() {
		cout << "����Ѽ��!" << endl;
	}
};
//ˮ������
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



//ˮ�����ǰ����ͬ
//���󹤳�
class AbstractFruitFactory {
public:
	virtual AbstractFruit* CreateFruit() = 0;
};

//ƻ������
class AppleFactory :public AbstractFruitFactory {
public:
	virtual AbstractFruit* CreateFruit() {
		return new Apple;
	}
};
//�㽶����
class BananaFactory :public AbstractFruitFactory {
public:
	virtual AbstractFruit* CreateFruit() {
		return new Banana;
	}
};
//Ѽ�湤��
class PearFactory :public AbstractFruitFactory {
public:
	virtual AbstractFruit* CreateFruit() {
		return new Pear;
	}
};

void test02() {
	AbstractFruitFactory* factory = nullptr;
	AbstractFruit* fruit = nullptr;

	//������Ӧ�ӹ���
	factory = new AppleFactory();
	fruit = factory->CreateFruit();
	fruit->ShowName();

	delete factory;
	delete fruit;
	//������Ӧ�ӹ���
	factory = new BananaFactory();
	fruit = factory->CreateFruit();
	fruit->ShowName();

	delete factory;
	delete fruit;
}

//����ϵͳ��
class AbstractOperationSystem {
public:
	virtual void welcome() = 0;
};
//�����ļ�ϵͳ��
class AbstractFileSystem {
public:
	virtual void FileSys() = 0;
};
//����CPU��
class AbstractCpu {
public:
	virtual void ShowType() = 0;
};
//�������������๹���˲�Ʒ��
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
//���������๹���˲���ϵͳ������Ĳ�Ʒ�ȼ�
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
//���������๹�����ļ�ϵͳ������Ĳ�Ʒ�ȼ�
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
//���������๹�����ļ�ϵͳ������Ĳ�Ʒ�ȼ�

//���󹤳� ʵ�������ֲ�Ʒ����������
class GUIFactory {
public:
	virtual std::unique_ptr<AbstractOperationSystem> LoadOperator() = 0;
	virtual std::unique_ptr<AbstractFileSystem> LoadFile() = 0;
	virtual std::unique_ptr<AbstractCpu> LoadCPU() = 0;
};
//���幤��1
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
//���幤��2
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

//���Ѿ�д�õ�ϵͳ
class Mysystem :public AbstractCommonInterface {
public:
	virtual void run() override {
		cout << "ϵͳ����..." << endl;
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
		//��֤�ڶ��߳��е�����ȫ
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
			cout << "�û�����������ȷ����֤ͨ��..." << endl;
			if (this->pSystem == nullptr) {
				pSystem = new Mysystem();
			}
			this->pSystem->run();
		}
		else {
			cout << "�û������������Ȩ�޲���...." << endl;
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
	//�������С��Ǹ��˶�������
	Mysystem* system = new Mysystem;
	system->run();
#endif
	//���ô���ģʽ
	std::shared_ptr<MysystemProxy>proxy = MysystemProxy::Instance("root","admin");
	proxy->run();
}

//���ȵĳ�����
class AbstractCoffee {
public:
	virtual std::string getDescription() const = 0;
	virtual double getCost()const = 0;
};
//���ȵ�һ��������
class SimpleCoffee :public AbstractCoffee{
public:
	virtual std::string getDescription() const override  {
		return "Simple Coffee";
	}
	virtual double getCost() const override {
		return 2.0;
	}
};
//װ����Ļ���
class CoffeeDecorator : public AbstractCoffee {
protected:
	AbstractCoffee* decoratedCoffee;
public:
	CoffeeDecorator(AbstractCoffee* coffee) : decoratedCoffee(coffee) {}
	virtual ~CoffeeDecorator() {
		delete decoratedCoffee;
	}
};
//װ�����һ������ ��ţ�̵Ŀ���
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
//װ�����һ������ ���ǵĿ���
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



//���ӻ�
class Televison {
public:
	void On() {
		cout << "���ӻ���..." << endl;
	}
	void Off() {
		cout << "���ӻ��ر�" << endl;
	}
};
//��
class Light {
public:
	void On() {
		cout << "�ƴ�..." << endl;
	}
	void Off() {
		cout << "�ƹر�" << endl;
	}
};
//����
class Audio {
public:
	void On() {
		cout << "�����..." << endl;
	}
	void Off() {
		cout << "����ر�" << endl;
	}
};
//��˷�
class Microphone {
public:
	void On() {
		cout << "��˷��..." << endl;
	}
	void Off() {
		cout << "��˷�ر�" << endl;
	}
};
//DVD
class DVDPlayer {
public:
	void On() {
		cout << "DVD��������..." << endl;
	}
	void Off() {
		cout << "DVD�������ر�" << endl;
	}
};
//��Ϸ��
class Gamemachine {
public:
	void On() {
		cout << "��Ϸ����..." << endl;
	}
	void Off() {
		cout << "��Ϸ���ر�" << endl;
	}
};

//KTVģʽ
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




// �ɵĴ�ӡϵͳ�ӿ�
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
// �µĴ�ӡϵͳʵ��
class NewPrinterImpl : public NewPrinter {
public:
	void printNewFormat(const string& newText) override {
		cout << "New Printer: " << newText << endl;
	}
};
// ��������
class PrinterAdapter : public NewPrinter {
private:
	OldPrinter* oldPrinter;
public:
	PrinterAdapter(OldPrinter* oldPrinter) : oldPrinter(oldPrinter) {}

	void printNewFormat(const string& newText) override {
		// ���¸�ʽ���ı�ת��Ϊ�ɸ�ʽ
		string oldText = "Adapted: " + newText;
		oldPrinter->printOldFormat(oldText);
	}
};
void test07() {
	OldPrinter* oldPrinter = new OldPrinter();
	NewPrinter* newPrinter = new NewPrinterImpl();

	// ͨ��������ʹ�þɵĴ�ӡϵͳ��ӡ�¸�ʽ���ı�
	NewPrinter* adapter = new PrinterAdapter(oldPrinter);

	string newText = "This is new format text.";

	cout << "Using New Printer directly:" << endl;
	newPrinter->printNewFormat(newText);

	cout << "Using Adapter with Old Printer:" << endl;
	adapter->printNewFormat(newText);

	// �ͷ��ڴ�
	delete oldPrinter;
	delete newPrinter;
	delete adapter;
}


class DrinkTemplate {
public:
	//עˮ
	virtual void Boildwater() = 0;
	//����
	virtual void Brew() = 0;
	//���뱭��
	virtual void PourInCup() = 0;
	//�Ӹ�����
	virtual void AddSomething() = 0;

	//ģ�巽��
	void Make() {
		Boildwater();
		Brew();
		PourInCup();
		AddSomething();
	}
};

//���ݿ���
class Coffee : public DrinkTemplate {
public:
	//עˮ
	virtual void Boildwater() {
		cout << "��ɽȪˮ..." << endl;
	}
	//����
	virtual void Brew() {
		cout << "���ݿ���..." << endl;
	}
	//���뱭��
	virtual void PourInCup() {
		cout << "���ȵ��뱭��..." << endl;
	}
	//�Ӹ�����
	virtual void AddSomething() {
		cout << "���ǣ���ţ�̣��ӵ��..." << endl;
	}
};

//���ݿ���
class Tea : public DrinkTemplate {
public:
	//עˮ
	virtual void Boildwater() {
		cout << "������ˮ..." << endl;
	}
	//����
	virtual void Brew() {
		cout << "����������..." << endl;
	}
	//���뱭��
	virtual void PourInCup() {
		cout << "��ˮ���뱭��..." << endl;
	}
	//�Ӹ�����
	virtual void AddSomething() {
		cout << "���ǣ������ʣ�������..." << endl;
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





//Э�鴦����
class HandleClientProtocol {
public:
	//�������ӽ��
	void AddMoney() {
		cout << "��������ӽ�ң�" << endl;
	}
	//����������ʯ
	void AddDiamod() {
		cout << "�����������ʯ��" << endl;
	}
	//�������װ��
	void AddEquipment() {
		cout << "����Ҵ�װ����" << endl;
	}
	//�����������
	void AddLevel() {
		cout << "�����������" << endl;
	}
};

//����ӿ�
class AbstractCommand {
public:
	virtual void handle() = 0; //����ͻ�������Ľӿ�
};

//�������ӽ������
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

//����������ʯ������
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

//�������װ��װ��������
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

//�����������������
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

//����������
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
	//�ͻ������ӽ�ҵ�����
	AbstractCommand* addmoney = new AddMoneyCommand(protocol);
	//�ͻ���������ʯ������
	AbstractCommand* adddimond = new AddDimondCommnd(protocol);
	//�ͻ��˴�װ��������
	AbstractCommand* addequiment = new AddEquipmentCommnd(protocol);
	//�ͻ�������������
	AbstractCommand* addlevel = new AddLevelCommand(protocol);

	//���ͻ���������뵽����Ķ�����
	Server* server = new Server;
	server->addRequest(addmoney);
	server->addRequest(adddimond);
	server->addRequest(addequiment);
	server->addRequest(addlevel);

	//��������ʼ��������
	server->startHandle();
}



//�������� ��������
class WeaponStrategy {
public:
	virtual void UseWeapon() = 0;
};

class Knife : public WeaponStrategy {
public:
	virtual void UseWeapon() {
		cout << "ʹ��ذ��!" << endl;
	}
};

class AK47 : public WeaponStrategy {
public:
	virtual void UseWeapon() {
		cout << "ʹ��AK47!" << endl;
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
	//������ɫ
	Character* character = new Character;

	//��������
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


//�����Ӣ��
class AbstractHero {
public:
	virtual void Update() = 0;

};

class HeroA :public AbstractHero {
public:
	HeroA() {
		cout << "Ӣ��A����ߣBoss..." << endl;
	}
	virtual void Update() {
		cout << "Ӣ��Aֹͣߣ������״̬..." << endl;
	}
};
class HeroB :public AbstractHero {
public:
	HeroB() {
		cout << "Ӣ��B����ߣBoss..." << endl;
	}
	virtual void Update() {
		cout << "Ӣ��Bֹͣߣ������״̬..." << endl;
	}
};
class HeroC :public AbstractHero {
public:
	HeroC() {
		cout << "Ӣ��C����ߣBoss..." << endl;
	}
	virtual void Update() {
		cout << "Ӣ��Cֹͣߣ������״̬..." << endl;
	}
};
class HeroD :public AbstractHero {
public:
	HeroD() {
		cout << "Ӣ��D����ߣBoss..." << endl;
	}
	virtual void Update() {
		cout << "Ӣ��Dֹͣߣ������״̬..." << endl;
	}
};
class HeroE :public AbstractHero {
public:
	HeroE() {
		cout << "Ӣ��E����ߣBoss..." << endl;
	}
	virtual void Update() {
		cout << "Ӣ��Eֹͣߣ������״̬..." << endl;
	}
};

//�۲�Ŀ�����
class AbstractBoss {
public:
	//��ӹ۲���
	virtual void addHero(AbstractHero* hero) = 0;
	//ɾ���۲���
	virtual void deleteHero(AbstractHero* hero) = 0;
	//֪ͨ���й۲���
	virtual void notify() = 0;
};

//����Ĺ۲��� BOSSA
class BOSSA :public AbstractBoss {
public:
	//��ӹ۲���
	virtual void addHero(AbstractHero* hero) {
		pHeroList.push_back(hero);
	}
	//ɾ���۲���
	virtual void deleteHero(AbstractHero* hero) {
		pHeroList.remove(hero);
	}
	//֪ͨ���й۲���
	virtual void notify() {
		for (list<AbstractHero*>::iterator it = pHeroList.begin(); it != pHeroList.end(); it++) {
			(*it)->Update();
		}
	}
public:
	list<AbstractHero*> pHeroList;
};

void test11() {
	//�����۲���
	AbstractHero* heroA = new HeroA;
	AbstractHero* heroB = new HeroB;
	AbstractHero* heroC = new HeroC;
	AbstractHero* heroD = new HeroD;
	AbstractHero* heroE = new HeroE;

	//�����۲�Ŀ��
	AbstractBoss* bossA = new BOSSA;
	bossA->addHero(heroA);
	bossA->addHero(heroB);
	bossA->addHero(heroC);
	bossA->addHero(heroD);
	bossA->addHero(heroE);

	//ģ����Ƽ��ܷ��� �������۲��߲��ܷ�������
	bossA->notify();
}

int main() {
	//�򵥹���ģʽ
	//test01();
	//��������ģʽ
	//test02();
	//���󹤳�ģʽ
	//test03();
	//����ģʽ
	//test04();
	//װ��ģʽ
	//test05();
	//���ģʽ
	//test06();
	//������ģʽ
	//test07();
	//ģ�巽��ģʽ
	//test08();
	//����ģʽ
	//test09();
	//����ģʽ
	//test10();
	//�۲���ģʽ
	test11();
	return 0;
}