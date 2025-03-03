#include <gtest/gtest.h>
#include "justadb/ddl.h"

TEST(DDLTest, DdlQueriesTest) {
    using namespace JustADb;
    const auto DB_NAME = "test_db";

    CreateDatabaseQuery create_db_query(DB_NAME);
    DatabaseManager db_manager;
    EXPECT_FALSE(db_manager.GetDatabase(DB_NAME)) << "Database already exists";

    DdlQueryExec ddl_query_exec(&db_manager);
    ddl_query_exec.ExecuteCreateDatabaseQuery(create_db_query);
    EXPECT_TRUE(db_manager.GetDatabase(DB_NAME)) << "Database not created";

    UseDatabaseQuery use_db_query(DB_NAME);
    ddl_query_exec.ExecuteUseDatabaseQuery(use_db_query);
    EXPECT_TRUE(db_manager.current_database()) << "Current database not set";
    EXPECT_EQ(DB_NAME, db_manager.current_database().value()->name()) << "Current database name mismatch";

    const auto TABLE_NAME = "test_table";
    const auto COLUMN_NAME = "column_1";

    const auto* column_1 = new Column(COLUMN_NAME, Column::Type::INT);
    CreateTableQuery create_table_query(TABLE_NAME, {column_1});
    const auto table_from_exec_query = ddl_query_exec.ExecuteCreateTableQuery(create_table_query);
    EXPECT_TRUE(table_from_exec_query.has_value()) << "Table not retured from executing create table query";

    const auto table_from_db_access = db_manager.current_database().value()->GetTable(TABLE_NAME);
    EXPECT_TRUE(table_from_db_access) << "Table not found in database";
    EXPECT_EQ(TABLE_NAME, table_from_db_access.value()->name()) << "Table name mismatch";

    const auto column_from_db_access = table_from_db_access.value()->GetColumn(COLUMN_NAME);
    EXPECT_TRUE(column_from_db_access) << "Column not found in table";
    EXPECT_EQ(COLUMN_NAME, column_from_db_access.value()->name()) << "Column name mismatch";

    delete column_1;
}
