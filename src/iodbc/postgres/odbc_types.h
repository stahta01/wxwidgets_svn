#define SQL_SPEC_STRING         "02.00"         /* String constant for version */
#define SQL_ACTIVE_CONNECTIONS               0
#define SQL_ACTIVE_STATEMENTS                1
#define SQL_DATA_SOURCE_NAME                 2
#define SQL_DATABASE_NAME               16    /* Use SQLGetConnectOption/SQL_CURRENT_QUALIFIER */
#define SQL_DBMS_VER                        18
#define SQL_FETCH_DIRECTION                  8
#define SQL_ROW_UPDATES                     11
#define SQL_ODBC_SAG_CLI_CONFORMANCE        12
#define SQL_DRIVER_VER                       7
#define SQL_SERVER_NAME                     13
#define SQL_SEARCH_PATTERN_ESCAPE           14
#define SQL_ODBC_API_CONFORMANCE             9
#define SQL_ODBC_SQL_CONFORMANCE            15
#define SQL_OAC_LEVEL1                      0x0001
#define SQL_DBMS_NAME                       17
#define SQL_ACCESSIBLE_PROCEDURES           20
#define SQL_OUTER_JOINS                     38
#define SQL_NEED_LONG_DATA_LEN              111
#define SQL_EXPRESSIONS_IN_ORDERBY          27
#define SQL_PROCEDURES                      21
#define SQL_COLUMN_ALIAS                    87
#define SQL_CONCAT_NULL_BEHAVIOR            22
#define SQL_DATA_SOURCE_READ_ONLY           25
#define SQL_ACCESSIBLE_TABLES               19
#define SQL_IDENTIFIER_QUOTE_CHAR           29
#define SQL_MAX_COLUMN_NAME_LEN             30
#define SQL_MAX_CURSOR_NAME_LEN             31
#define SQL_MAX_OWNER_NAME_LEN              32
#define SQL_MAX_PROCEDURE_NAME_LEN          33
#define SQL_MAX_QUALIFIER_NAME_LEN          34
#define SQL_MAX_TABLE_NAME_LEN              35
#define SQL_MULT_RESULT_SETS                36
#define SQL_MULTIPLE_ACTIVE_TXN             37
#define SQL_MAX_ROW_SIZE_INCLUDES_LONG      103
#define SQL_OWNER_TERM                      39
#define SQL_PROCEDURE_TERM                  40
#define SQL_QUALIFIER_NAME_SEPARATOR        41
#define SQL_QUALIFIER_TERM                  42
#define SQL_SCROLL_CONCURRENCY              43
#define SQL_SCROLL_OPTIONS                  44
#define SQL_TABLE_TERM                      45
#define SQL_TXN_CAPABLE                     46
#define SQL_USER_NAME                       47
#define SQL_CONVERT_FUNCTIONS               48
#define SQL_SYSTEM_FUNCTIONS                51
#define SQL_NUMERIC_FUNCTIONS               49
#define SQL_FN_NUM_ABS                      0x00000001L
#define SQL_FN_NUM_FLOOR                    0x00000200L
#define SQL_FN_NUM_MOD                      0x00000800L
#define SQL_FN_NUM_SIGN                     0x00001000L
#define SQL_STRING_FUNCTIONS                50
#define SQL_FN_STR_CONCAT                   0x00000001L
#define SQL_FN_STR_LEFT                     0x00000004L
#define SQL_FN_STR_LENGTH                   0x00000010L
#define SQL_FN_STR_LOCATE                   0x00000020L
#define SQL_FN_STR_LOCATE_2                 0x00010000L
#define SQL_FN_STR_RIGHT                    0x00000200L
#define SQL_FN_STR_SUBSTRING                0x00000800L
#define SQL_FN_STR_REPLACE                  0x00000100L
#define SQL_FN_STR_LCASE                    0x00000040L
#define SQL_FN_STR_UCASE                    0x00001000L
#define SQL_TIMEDATE_FUNCTIONS              52
#define SQL_FN_TD_CURDATE                   0x00000002L
#define SQL_CONVERT_BIGINT                  53
#define SQL_CONVERT_BIT                     55
#define SQL_CONVERT_CHAR                    56
#define SQL_CONVERT_DATE                    57
#define SQL_CONVERT_DECIMAL                 58
#define SQL_CONVERT_DOUBLE                  59
#define SQL_CONVERT_FLOAT                   60
#define SQL_CONVERT_INTEGER                 61
#define SQL_CONVERT_LONGVARCHAR             62
#define SQL_CONVERT_NUMERIC                 63
#define SQL_CONVERT_REAL                    64
#define SQL_CONVERT_SMALLINT                65
#define SQL_CONVERT_TIME                    66
#define SQL_CONVERT_TIMESTAMP               67
#define SQL_CONVERT_TIMESTAMP               67
#define SQL_CONVERT_TINYINT                 68
#define SQL_CONVERT_VARCHAR                 70
#define SQL_CVT_CHAR                        0x00000001L
#define SQL_CVT_NUMERIC                     0x00000002L
#define SQL_CVT_DECIMAL                     0x00000004L
#define SQL_CVT_INTEGER                     0x00000008L
#define SQL_CVT_SMALLINT                    0x00000010L
#define SQL_CVT_FLOAT                       0x00000020L
#define SQL_CVT_REAL                        0x00000040L
#define SQL_CVT_DOUBLE                      0x00000080L
#define SQL_CVT_VARCHAR                     0x00000100L
#define SQL_CVT_LONGVARCHAR                 0x00000200L
#define SQL_CVT_BIT                         0x00001000L
#define SQL_CVT_TINYINT                     0x00002000L
#define SQL_CVT_BIGINT                      0x00004000L
#define SQL_CVT_DATE                        0x00008000L
#define SQL_CVT_TIME                        0x00010000L
#define SQL_CVT_TIMESTAMP                   0x00020000L
#define SQL_CVT_TIMESTAMP                   0x00020000L
#define SQL_CONVERT_BINARY                  54
#define SQL_CONVERT_VARBINARY               69
#define SQL_CONVERT_LONGVARBINARY           71
#define SQL_CORRELATION_NAME                74
#define SQL_CN_DIFFERENT                    0x0001
#define SQL_NNC_NON_NULL                    0x0001
#define SQL_NULL_COLLATION                  85
#define SQL_NC_START                        0x0002
#define SQL_MAX_COLUMNS_IN_GROUP_BY         97
#define SQL_MAX_COLUMNS_IN_ORDER_BY         99
#define SQL_MAX_COLUMNS_IN_SELECT           100
#define SQL_MAX_COLUMNS_IN_TABLE            101
#define SQL_MAX_TABLES_IN_SELECT            106
#define SQL_MAX_ROW_SIZE_INCLUDES_LONG      103
#define SQL_MAX_ROW_SIZE                    104
#define SQL_MAX_BINARY_LITERAL_LEN          112
#define SQL_MAX_CHAR_LITERAL_LEN            108
#define SQL_MAX_COLUMNS_IN_INDEX            98
#define SQL_MAX_INDEX_SIZE                  102
#define SQL_MAX_STATEMENT_LEN               105
#define SQL_QL_START                        0x0001L
#define SQL_SEARCHABLE                  3
#define SQL_IDENTIFIER_CASE                 28
#define SQL_COLUMN_NAME                 1
#define SQL_COLUMN_TYPE                 2
#define SQL_COLUMN_TYPE_NAME            14
#define SQL_COLUMN_PRECISION            4
#define SQL_COLUMN_DISPLAY_SIZE         6
#define SQL_COLUMN_LENGTH               3
#define SQL_COLUMN_SCALE                5
#define SQL_COLUMN_NULLABLE             7
#define SQL_COLUMN_SEARCHABLE           13
#define SQL_COLUMN_UNSIGNED             8
#define SQL_COLUMN_MONEY                9
#define SQL_COLUMN_AUTO_INCREMENT       11
#define SQL_COLUMN_CASE_SENSITIVE       12
#define SQL_COLUMN_UPDATABLE            10
#define SQL_COLUMN_OWNER_NAME           16
#define SQL_COLUMN_QUALIFIER_NAME       17
#define SQL_OSCC_COMPLIANT                  0x0001
#define SQL_ODBC_SQL_OPT_IEF                73
#define SQL_LIKE_ESCAPE_CLAUSE              113
#define SQL_ORDER_BY_COLUMNS_IN_SELECT      90
#define SQL_POS_OPERATIONS                  79
#define SQL_POSITIONED_STATEMENTS           80
#define SQL_LOCK_TYPES                      78
#define SQL_BOOKMARK_PERSISTENCE            82
#define SQL_ALTER_TABLE                     86
#define SQL_OWNER_USAGE                     91
#define SQL_QUALIFIER_USAGE                 92
#define SQL_QUOTED_IDENTIFIER_CASE          93
#define SQL_SUBQUERIES                      95
#define SQL_UNION                           96
#define SQL_TIMEDATE_DIFF_INTERVALS         110
#define SQL_GETDATA_EXTENSIONS              81
#define SQL_GD_ANY_COLUMN                   0x00000001L
#define SQL_GD_ANY_ORDER                    0x00000002L
#define SQL_GD_BOUND                        0x00000008L
#define SQL_STATIC_SENSITIVITY              83
#define SQL_SS_DELETIONS                    0x00000002L
#define SQL_SS_UPDATES                      0x00000004L
#define SQL_FILE_USAGE                      84
#define SQL_FILE_NOT_SUPPORTED              0x0000
#define SQL_GROUP_BY                        88
#define SQL_GB_GROUP_BY_EQUALS_SELECT       0x0001
#define SQL_KEYWORDS                        89
#define SQL_SPECIAL_CHARACTERS              94
#define SQL_TIMEDATE_ADD_INTERVALS          109
#define SQL_FN_SYS_DBNAME                   0x00000002L
#define SQL_FN_SYS_IFNULL                   0x00000004L
#define SQL_FN_SYS_USERNAME                 0x00000001L
#define SQL_FN_NUM_CEILING                  0x00000020L
#define SQL_FN_NUM_EXP                      0x00000100L
#define SQL_FN_NUM_LOG                      0x00000400L
#define SQL_FN_NUM_LOG10                    0x00080000L
#define SQL_FN_NUM_LOG10                    0x00080000L
#define SQL_FN_NUM_POWER                    0x00100000L
#define SQL_FN_NUM_SQRT                     0x00004000L
#define SQL_FN_NUM_ROUND                    0x00400000L
#define SQL_FN_STR_INSERT                   0x00000002L
#define SQL_FN_STR_LTRIM                    0x00000008L
#define SQL_FN_STR_RTRIM                    0x00000400L
#define SQL_BLOB                          21
#define SQL_BLOB_LOCATOR                  22
#define SQL_CLOB                          23
#define SQL_CLOB_LOCATOR                  24
#define SQL_DBCLOB                        25
#define SQL_DBCLOB_LOCATOR                26
#define SQL_GRAPHIC                       27
#define SQL_LONGVARGRAPHIC                30
#define SQL_VARGRAPHIC                    32
#define SQL_SQLSTATE_SIZE                8
#define SQL_COLUMN_DISTINCT_TYPE        1250
#define SQL_COLUMN_TABLE_NAME             15
#define SQL_LEN_DATA_AT_EXEC_OFFSET  (-100)
#define SQL_CB_NULL                         0x0000
#define SQL_FN_NUM_RAND                     0x00020000L
#define SQL_NOSCAN_OFF                  0UL     
#define SQL_ASYNC_ENABLE_OFF            0UL
#define SQL_CURSOR_STATIC               3UL
#define SQL_SC_NON_UNIQUE               0UL
#define SQL_UB_OFF                      0UL
#define SQL_PC_NOT_PSEUDO               1
#define SQL_PC_NON_PSEUDO               SQL_PC_NOT_PSEUDO
#define SQL_UNSEARCHABLE                0

/*
 * Following constants are missed in original odbc_types.h
 * Added by serg@informika.ru
 */

#define SQL_UNKNOWN_TYPE		0

/* SQLColAttributes subdefines for SQL_COLUMN_SEARCHABLE */
/* These are also used by SQLGetInfo                     */
#define SQL_UNSEARCHABLE                0
#define SQL_LIKE_ONLY                   1
#define SQL_ALL_EXCEPT_LIKE             2
#define SQL_SEARCHABLE                  3
#define SQL_PRED_SEARCHABLE				SQL_SEARCHABLE

/* Special return values for SQLGetData */
#define SQL_NO_TOTAL                    (-4)

/* SQL_CORRELATION_NAME values */

#define SQL_CN_NONE                         0x0000
#define SQL_CN_DIFFERENT                    0x0001
#define SQL_CN_ANY                          0x0002

/* SQL_NULL_COLLATION values */

#define SQL_NC_HIGH                         0
#define SQL_NC_LOW                          1
#define SQL_NC_START                        0x0002
#define SQL_NC_END                          0x0004

/* SQL_GROUP_BY values */

#define SQL_GB_NOT_SUPPORTED                0x0000
#define SQL_GB_GROUP_BY_EQUALS_SELECT       0x0001
#define SQL_GB_GROUP_BY_CONTAINS_SELECT     0x0002
#define SQL_GB_NO_RELATION                  0x0003

/* SQL_IDENTIFIER_CASE values */
#define SQL_IC_UPPER                        1
#define SQL_IC_LOWER                        2
#define SQL_IC_SENSITIVE                    3
#define SQL_IC_MIXED                        4

/* SQL_ODBC_SQL_CONFORMANCE values */

#define SQL_OSC_MINIMUM                     0x0000
#define SQL_OSC_CORE                        0x0001
#define SQL_OSC_EXTENDED                    0x0002

/* SQL_SCROLL_OPTIONS masks */

#define SQL_SO_FORWARD_ONLY                 0x00000001L
#define SQL_SO_KEYSET_DRIVEN                0x00000002L
#define SQL_SO_DYNAMIC                      0x00000004L
#define SQL_SO_MIXED                        0x00000008L
#define SQL_SO_STATIC                       0x00000010L

/* SQL_TXN_CAPABLE values */

#define SQL_TC_NONE                         0
#define SQL_TC_DML                          1
#define SQL_TC_ALL                          2
#define SQL_TC_DDL_COMMIT                   3
#define SQL_TC_DDL_IGNORE                   4

/* SQL_ALTER_TABLE bitmasks */

#if (ODBCVER >= 0x0200)
#define SQL_AT_ADD_COLUMN                   	0x00000001L
#define SQL_AT_DROP_COLUMN                  	0x00000002L
#endif /* ODBCVER >= 0x0200 */

#define SQL_MAX_USER_NAME_LEN               107

/* SQLColAttributes subdefines for SQL_COLUMN_UPDATABLE */

#define SQL_ATTR_READONLY               0
#define SQL_ATTR_WRITE                  1
#define SQL_ATTR_READWRITE_UNKNOWN      2

/* SQLExtendedFetch "rgfRowStatus" element values */

#define SQL_ROW_SUCCESS                  0
#define SQL_ROW_DELETED                  1
#define SQL_ROW_UPDATED                  2
#define SQL_ROW_NOROW                    3
#define SQL_ROW_ADDED                    4
#define SQL_ROW_ERROR                    5

/* SQL_TIMEDATE_FUNCTIONS */

#define SQL_FN_TD_NOW			0x00000001L
#define SQL_FN_TD_CURTIME		0x00000200L
