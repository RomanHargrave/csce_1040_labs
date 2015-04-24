#include "parse.h"

stack theStack;

DBrecord *makeBlankRecord()
{
    DBrecord *dbr;
    dbr = (DBrecord *) malloc(sizeof(DBrecord));
    dbr->bitID = 0;
    dbr->lName = NULL;
    dbr->fName = NULL;
    strcpy(dbr->idNum,"0");
    dbr->age = 0;
    dbr->year = 0;
    dbr->gpa = 0.00;
    dbr->gradYear = 0;

    return dbr;
}
Field findAttribute(char *buffer)
{
    if( (strcmp(buffer,"lastName")) == 0 )
        return LastName;
    else if( (strcmp(buffer,"firstName")) == 0 )
        return FirstName;
    else if( (strcmp(buffer,"idNum")) == 0 )
        return ID;
    else if( (strcmp(buffer,"age")) == 0 )
        return Age;
    else if( (strcmp(buffer,"year")) == 0 )
        return Year;
    else if( (strcmp(buffer,"gpa")) == 0 )
        return GPA;
    else if( (strcmp(buffer,"expectedGrad")) == 0 )
        return GradYear;
    else
    {
        printf("Error in findAttribute --- value %s not found\n",buffer);
        assert(0);
    }
}

void findRecordIndices(Field thisField, 
                       char oper, 
                       char *value, 
                       int *low, 
                       int *high)
{
    // find the range of records in the appropriate sorted array of
    // theDataBase, and return the low and high indices of those records

    // first, convert the "value" field to an appropriate type.

    DBrecord *dbr;
    int intValue;
    double floatValue;
    int l,h,index;

    dbr = makeBlankRecord();
    switch (thisField) 
    {
        case Age:
	    intValue = atoi(value);
            dbr->age = intValue;
            break;

        case Year:
            if( (strcmp(value,"firstYear") == 0) )
	        intValue = senior;
            else if( (strcmp(value,"sophomore") == 0) )
	        intValue = sophomore;
            else if( (strcmp(value,"junior") == 0) )
	        intValue = junior;
            else if( (strcmp(value,"senior") == 0) )
	        intValue = senior;
            else if( (strcmp(value,"grad") == 0) )
	        intValue = grad;
            else
            {
		printf("Error, unexpected year value\n");
		assert(0);
            }
            dbr->year = intValue;
            break;

        case GradYear:
	    intValue = atoi(value);
            dbr->gradYear = intValue;
            break;

        case GPA:
	    floatValue = atof(value);
            dbr->gpa = floatValue;
            break;

        case ID:
            strcpy(dbr->idNum, value); 
            break;

        case LastName:
	    dbr->lName = (char *) malloc(strlen(value));
            strcpy(dbr->lName,value);
            break;

        case FirstName:
	    dbr->fName = (char *) malloc(strlen(value));
            strcpy(dbr->fName,value);
            break;

	default:
	    printf("Error in switch statement. Unknown field value, %d\n",
                    thisField);
            assert(0);
    }

    index = binarySearch(theDataBase[thisField],dbr,thisField,0,319);
    assert(index != -1);
    switch (oper)
    {
	case '=':
            for(l = index; 
	        l >= 0 && compare(theDataBase[thisField][l],dbr,thisField) == 0;
	        l--)
			;  // empty for loop
            for(h = index; h <= 319 && 
                compare(theDataBase[thisField][h],dbr,thisField) == 0; 
	        h++)
			;  // empty for loop
            *low = l+1;
            *high = h-1;
            break;

	case '>':
	    while(compare(theDataBase[thisField][index],dbr,thisField) < 0)
		index++;
            for(l = index; 
	        l >= 0 && compare(theDataBase[thisField][l],dbr,thisField) > 0;
	        l--)
			;  // empty for loop
            for(h = index; h <= 319 && 
                compare(theDataBase[thisField][h],dbr,thisField) > 0; 
	        h++)
			;  // empty for loop
            *low = l+1;
            *high = h-1;
            break;
 
	case '<':
	    while(compare(theDataBase[thisField][index],dbr,thisField) > 0)
		index--;
            for(l = index; 
	        l >= 0 && compare(theDataBase[thisField][l],dbr,thisField) < 0;
	        l--)
			;  // empty for loop
            for(h = index; h <= 319 && 
                compare(theDataBase[thisField][h],dbr,thisField) < 0; 
	        h++)
			;  // empty for loop
            *low = l+1;
            *high = h-1;
            break;

	default:
	    printf("Bad value for operator = %c\n",oper);
            assert(0);
    }
}

Set *buildASet(Field thisField, char oper, char* value)
{
    Set *setPtr;
    int index,
        high,
        low;

    setPtr = (Set *) malloc(sizeof (Set));

    // find the records in theDataBase and add their bit ids
    // to *setPtr
  
    findRecordIndices(thisField,oper,value,&low,&high);
    for( index = low; index <= high; index++ )
    {
        add2Set(*setPtr,theDataBase[thisField][index]->bitID);
    }  
    return setPtr;
}

void buildAndPushASet(stack *s)
{
    char buffer[10];
    char operator[2];
    char value[20];
    Field attribute;
    Set *thisSet;

    scanf("%s",buffer);
    scanf("%s",operator);
    scanf("%s",value);
    attribute = findAttribute(buffer); 
    thisSet = buildASet(attribute,operator[0],value); 
    push(s,*thisSet);
}


void parseAndExecuteCommands()
{
    char buffer[80];
    Set *resultSet;
    stackNode *sn1;
    stackNode *sn2;

    initStack(&theStack);
    while ( (scanf("%s",buffer) != EOF) )
    {
        switch(buffer[0])
        {
            case 'c':
            case 'C':
                buildAndPushASet(&theStack);
                break;

	    case 'p':
	    case 'P':
	        sn1 = pop(&theStack);
                printSet(sn1->data);
                break;

	    case 'i':
	    case 'I':
                sn1 = pop(&theStack);
                sn2 = pop(&theStack);
                resultSet = (Set *) malloc(sizeof(Set));
                setIntersection(sn1->data, sn2->data, *resultSet);
                push(&theStack, *resultSet);
                break;

	    case 'u':
	    case 'U':
                sn1 = pop(&theStack);
                sn2 = pop(&theStack);
                resultSet = (Set *) malloc(sizeof(Set));
                setUnion(sn1->data, sn2->data, *resultSet);
                push(&theStack, *resultSet);
                break;
        }
    }
}
