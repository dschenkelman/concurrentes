#ifndef MESSAGE_QUEUE_H_
#define MESSAGE_QUEUE_H_

#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <stdio.h>

template <class T> class MessageQueue {
	private:
		key_t	key;
		int		id;

	public:
		MessageQueue ( char* file,char letter );
		~MessageQueue();
		int write ( T data );
		int read ( int type,T* buffer );
		int destroy ();
};

template <class T> MessageQueue<T> :: MessageQueue ( char* file,char letter ) {
	this->key = ftok ( file,letter );
	if ( this->key == -1 )
		perror ( "Ftok Error" );

	this->id = msgget ( this->key,0777|IPC_CREAT );
	if ( this->id == -1 )
		perror ( "Msgget Error" );
}

template <class T> MessageQueue<T> :: ~MessageQueue () {
}

template <class T> int MessageQueue<T> :: destroy () {
	int result = msgctl ( this->id,IPC_RMID,NULL );
	return result;
}

template <class T> int MessageQueue<T> :: write ( T data ) {
	int result = msgsnd ( this->id,(const void *)&data,sizeof(T)-sizeof(long),0 );
	return result;
}

template <class T> int MessageQueue<T> :: read ( int type,T* buffer ) {
	int result = msgrcv ( this->id,(void *)buffer,sizeof(T)-sizeof(long),type,0 );
	return result;
}

#endif /* MESSAGE_QUEUE_H_ */
