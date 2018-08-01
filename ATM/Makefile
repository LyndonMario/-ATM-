all:
	gcc client.c tool.c get_keyboard.c msg_queue.c -o client
	
	gcc server.c tool.c msg_queue.c get_keyboard.c -o server
	
	gcc open.c tool.c msg_queue.c get_keyboard.c -o open
	
	gcc destroy.c tool.c msg_queue.c get_keyboard.c -o destroy
	
	gcc login.c tool.c msg_queue.c get_keyboard.c -o login
	
	gcc unlock.c tool.c msg_queue.c get_keyboard.c -o unlock
	
	gcc query.c tool.c msg_queue.c get_keyboard.c -o query
	
	gcc transf.c tool.c msg_queue.c get_keyboard.c -o transf
	
	gcc take.c tool.c msg_queue.c get_keyboard.c -o take
	
	gcc save.c tool.c msg_queue.c get_keyboard.c -o save
	
	gcc modify.c tool.c msg_queue.c get_keyboard.c -o modify
clean:
	rm client server open save take query transf destroy
	
