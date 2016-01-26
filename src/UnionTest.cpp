//============================================================================
// Name        : UnionTest.cpp
// Author      : 
// Version     :
// Copyright   : My copyright notice
// Description : MAF in C++, Ansi-style
//============================================================================

using namespace std;
#include "UnionClient.h"
#include "connector/AbstractConnector.h"
#include "connector/CoutConnector.h"
#include "connector/StandardConnector.h"

#include <thread>
#include <chrono>
/*
class TestConnector: public AbstractConnector {
public:
	TestConnector() {}
	virtual ~TestConnector() {}

	virtual bool IsReady(){
		return true;
	}
	virtual int Connect(){
		return 0;
	}

	virtual int Disconnect(){
		return 0;
	}

	virtual void Send(std::string msg){
		cout << msg << endl;
	}

	virtual CnxRef GetInProgressConnection(){
		return nullptr;
	}

} testConnector;
*/
class CoutConnector testConnector;
class StandardUVConnector upcConnector("localhost", "9100");

class Thingummy: public Dispatcher<std::string,float>
{
public:
	Thingummy() {}

	void RunThisInAThread() {
		ProcessDispatches();
	}

	void CallThisFromOutside(EventType e, std::string s, float f)
	{
		DispatchEvent(e, s, f);
	}

} thing;

std::string prompt("> ");
int main(int argc, char **argv) {
	cout << "Testing 1 ... 2 .. 3\n";

	UnionClient unionClient(upcConnector);
	cout << "Testing 2 ... 3 .. 4\n";

//	unionClient.SetHeartbeatFrequency(60000);
//	unionClient.SetConnectionTimeout(60000);
//	unionClient.SetReadyTimeout(60000);
	// would be possible to add extra connections here, but the StandardConnector already implements everything
	//	upc.Connection(socketConnection);

	auto test1 = thing.AddEventListener(1, [] (EventType t, std::string f, float x) {
		std::cerr << "testing ... " << t << ", " << f << ", " << x << std::endl;
	});

	thing.CallThisFromOutside(1, "a string", 3.142);

	std::cerr << "ooo kkkk" << std::endl;

	thing.RunThisInAThread();

	bool ready = false;
	bool connectError = false;

	auto readyListenerHandle = unionClient.AddEventListener(Event::READY, [&ready] (EventType t, std::string a, UPCStatus s) {
		std::cerr << "test::got a ready from the server" << std::endl;
		ready = true;
	});
	auto disconnectedListenerHandle = unionClient.AddEventListener(Event::DISCONNECTED, [&ready] (EventType t, std::string a, UPCStatus s) {
		std::cerr << "test::disconnected from server, " + a + "..." << std::endl;
		ready = false;
	});
	auto selectConnectionListenerHandle = unionClient.AddEventListener(Event::SELECT_CONNECTION,[&ready] (EventType t, std::string a, UPCStatus s) {
		std::cerr << "test::new connection selected, " + a + "..." << std::endl;
	});
	auto connectFailureListenerHandle = unionClient.AddEventListener(Event::CONNECT_FAILURE, [&ready,&connectError] (EventType t, std::string a, UPCStatus s) {
		std::cerr << "test::connect failure, " + a + "..." << std::endl;
		ready = false;
		connectError = true;
	});

	cerr << "About to Connect ... \n";
	unionClient.Connect();
	cerr << "Connecting ...\n";
	for (;;) {
		if (ready) {
			break;
		} else if (connectError) {
			cerr << "Connect error ... kinda fatal .. bye!" << endl;
			return -1;
		}
	}
	cerr << "We've got a ready ...\n";

	unionClient.Self()->GetClientID();
	unionClient.Self()->SetAttribute("key", "value");
//	auto roomJoined = unionClient.Self()->NotifyRoomInfo::AddEventListener(Event::JOIN_ROOM,
//			[] (int, std::basic_string<char>, std::basic_string<char>, std::shared_ptr<const Room>, int) {
//		;
//	});

//	RoomRef tryAndFail = unionClient.GetRoomManager().CreateRoom("proba");
	std::cerr << "Inside the presentation loop ... I am number " << unionClient.Self()->GetClientID() << endl;
	CBClientInfoRef addMeetingOccupant;
	CBClientInfoRef removeMeetingOccupant;
	auto roomJoined = unionClient.Self()->NotifyClientInfo::AddEventListener(Event::JOIN_ROOM,
			[&addMeetingOccupant, &removeMeetingOccupant](EventType e,  ClientID, ClientRef, int, RoomID roomID, RoomRef  rr, UPCStatus s) {
		cerr << " joining " << roomID << endl;

		if (roomID == "lobby") {
			cerr << "Sending credentials.." << endl;
			rr->SendModuleMessage("login",
					{
							{"policy",  "b2lkPXJvb21uYW1lJmNhbl9lZGl0PTEmdXNlcm5hbWU9dXNlcm5hbWUmbmFtZT1QcmV6aStNb25pdG9yJnV0Yz0xNDg2NTY4MzUzLjk1JnJvb21fZW1wdHk9MQ=="},
							{"signature",  "0TSPohHP45eQPCgB6evBKL+PyaU="},
							{"currentVersion",  "0"},
							{"reconnecting", "false"},
							{"newReconnectFlow", "true"}
					});
		} else {
			cerr << "Authentication successful!" << endl;
			RoomRef meetingRoom = rr;
			addMeetingOccupant = meetingRoom->NotifyClientInfo::AddEventListener(Event::ADD_OCCUPANT,
					[](EventType e, ClientID clientID, ClientRef, int, RoomID roomID, RoomRef  rr, UPCStatus s) {
							cerr << roomID << " adding occupant " << clientID  << endl;
						});
			removeMeetingOccupant = meetingRoom->NotifyClientInfo::AddEventListener(Event::REMOVE_OCCUPANT,
					[](EventType e, ClientID clientID, ClientRef, int, RoomID roomID, RoomRef  rr, UPCStatus s) {
							cerr<< roomID << " removing occupant " << clientID  << endl;
						});
		}
	});
	auto roomLeft = unionClient.Self()->NotifyClientInfo::AddEventListener(Event::LEAVE_ROOM,
			[](EventType e, ClientID, ClientRef, int, RoomID roomID, RoomRef  rr, UPCStatus s) {
		cerr << " leaving " << roomID << endl;
	});

	auto attributeChanged = unionClient.Self()->NotifyAttrInfo::AddEventListener(Event::UPDATE_ATTR,
		[](EventType t, AttrName n, AttrVal v, AttrScope s, AttrVal ov, ClientRef c, UPCStatus status) {
			cerr << "attrib " << n << " -> " << v << " in scope "<<s <<" for client " <<(c?c->GetName():"senki") << endl;
		});
	RoomRef theRoom = unionClient.GetRoomManager().CreateRoom("theRoom");
	RoomRef lobby = unionClient.GetRoomManager().JoinRoom("lobby");
	lobby->AddMessageListener("MODULE_MSG", [] (UserMessageID mid,/* RoomID room, */StringArgs msg) {
		std::cerr << std::endl;
		for (auto it: msg) {
			std::cerr << it << std::endl;
		}
		std::cerr << std::endl;
	});
	lobby->AddMessageListener("CHAT_MESSAGE", [] (UserMessageID mid,/* RoomID room, */StringArgs msg) {
		std::cerr << std::endl;
		for (auto it: msg) {
			std::cerr << it << std::endl;
		}
		std::cerr << std::endl;
	});
	lobby->AddMessageListener("COMMAND", [] (UserMessageID mid,/* RoomID room, */StringArgs msg) {
		std::cerr << std::endl;
		for (auto it: msg) {
			std::cerr << it << std::endl;
		}
		std::cerr << std::endl;
	});

	std::cerr << "added message listener " << unionClient.Self()->GetClientID() << endl;

	std::string cmdIn;
	std::cout << prompt;
	std::getline(cin, cmdIn);

	cout << "Testing 3 ... 2 .. 1\n";
// to remove just one call back using 'roomJoined'
//	unionClient.Self()->NotifyRoomInfo::RemoveListener(Event::JOIN_ROOM, roomJoined);
// or ... this will remove all listeners using 'roomJoined'
//	roomJoined = nullptr; // only a weak pointer is stored ... all event listens will be auto deleted.

	return 0;
}
