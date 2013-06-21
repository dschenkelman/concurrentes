package Concurrentes.ClientGUI;

public enum RequestActionType {
	CREATE("Create",1),UPDATE("Update", 2),
	READ("Read", 3),DELETE("Delete", 4),HEAD("Head", 5),
	OPERATION_FAILED("Operation Failed", 6),ENDOFCONNECTION("End of Connection", 7),
	OPERATION_CREATE_SUCCESS("Operation Create Success", 8),
	OPERATION_UPDATE_SUCCESS("Operation Update Success", 9),
	OPERATION_DELETE_SUCCESS("Operation Delete Success", 10),
	OPERATION_UNKNOWN("Operation unknown", 11),
	BODY("Body", 12),GRACEFUL_QUIT("Graceful Quit", 13),
	NULL_ACTION_TYPE("Null Action Type", 14);
	
    private String description;
    private int id;

    RequestActionType(String name, int id) {
        this.description = name;
        this.id = id;
    }

    @Override
    public String toString() {
    	
        return this.description;
    }

    public int getId() {
        return this.id;
    }
    
    public static RequestActionType valueOfByDescription( String description ){
    	for( RequestActionType type : values() ){
    		if( type.description.equals(description))
    			return type;
    	}
		return null;
    }
}
