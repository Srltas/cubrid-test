/*
 * Copyright (C) 2008 Search Solution Corporation. 
 * Copyright (c) 2016 CUBRID Corporation.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 *
 * - Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * - Neither the name of the <ORGANIZATION> nor the names of its contributors
 *   may be used to endorse or promote products derived from this software without
 *   specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 */

#include "cubrid.h"

extern VALUE cConnection;

extern VALUE cubrid_stmt_new(Connection *con, char *stmt, int option);
extern VALUE cubrid_stmt_execute(int argc, VALUE* argv, VALUE self);
extern VALUE cubrid_stmt_fetch(VALUE self);
extern VALUE cubrid_stmt_close(VALUE self);
extern VALUE cubrid_oid_new(Connection *con, char *oid_str);

static void
cubrid_conn_free(void *p) 
{
  Connection *c = (Connection *)p;
  T_CCI_ERROR error;

  if (c->handle) {
    cci_disconnect(c->handle, &error);
    c->handle = 0;
  }

  free(p);
}

VALUE 
cubrid_conn_new(char *host, int port, char *db, char *user, char *passwd)
{
  VALUE conn;
  Connection *c;
  int handle;

  handle = cci_connect(host, port, db, user, passwd);
  if (handle < 0) {
    cubrid_handle_error(handle, NULL);
    return Qnil;
  }

  conn = Data_Make_Struct(cConnection, Connection, 0, cubrid_conn_free, c);

  c->handle = handle;
  strcpy(c->host, host);
  c->port = port;
  strcpy(c->db, db);
  strcpy(c->user, user);
  c->auto_commit = Qfalse;

  return conn;
}

/* call-seq:
 *   close() -> nil
 *
 * ???????????????????????? ????????? ???????????????. ??? ??????????????? ????????? Statement??? ?????? close?????????.
 */
VALUE 
cubrid_conn_close(VALUE self)
{
  Connection *c;
  T_CCI_ERROR error;

  GET_CONN_STRUCT(self, c);

  if (c->handle) {
    cci_disconnect(c->handle, &error);
    c->handle = 0;
  }

  return Qnil;
}

static VALUE 
cubrid_conn_prepare_internal(int argc, VALUE* argv, VALUE self)
{
  Connection *con;
  VALUE sql, option;

  GET_CONN_STRUCT(self, con);
  CHECK_CONNECTION(con, Qnil);

  rb_scan_args(argc, argv, "11", &sql, &option);

  if (NIL_P(sql)) {
    rb_raise(rb_eStandardError, "SQL is required.");
  }

  if (NIL_P(option)) {
    option = INT2NUM(0);
  }

  return cubrid_stmt_new(con, StringValueCStr(sql), NUM2INT(option));
}

/* call-seq:
 *   prepare(sql <, option>) -> Statement
 *   prepare(sql <, option>) { |stmt| block } -> nil
 *
 * ????????? SQL??? ????????? ????????? ?????? Statement ????????? ???????????????. 
 * SQL??? ?????????????????? ????????? ????????? ???????????? ????????? ??? ????????? ???????????????.
 *
 * option?????? Cubrid::INCLUDE_OID??? ??? ??? ?????????, ????????? SQL ?????? ????????? OID??? ??????????????? ?????????.
 * ?????? ????????? ????????? OID??? Statement.get_oid ???????????? ?????? ??? ????????????.
 *
 * block ???????????? ????????? Statement ????????? ????????? ???????????? block??? ??????????????????. 
 * block??? ????????? ????????? Statement ????????? ????????? ???????????? ????????????.
 *
 *  con = Cubrid.connect('demodb')
 *  stmt = con.prepare('SELECT * FROM db_user')
 *  stmt.execute
 *  r = stmt.fetch
 *  stmt.close
 *  con.close
 *
 *  con.prepare('SELECT * FROM db_user') { |stmt|
 *    stmt.execute
 *    r = stmt.fetch
 *  }
 *  con.close
 *
 */
VALUE 
cubrid_conn_prepare(int argc, VALUE* argv, VALUE self)
{
  VALUE stmt;
  Connection *con;

  GET_CONN_STRUCT(self, con);

  stmt = cubrid_conn_prepare_internal(argc, argv, self);
  
  if (rb_block_given_p()) {
    rb_yield(stmt);
    cubrid_stmt_close(stmt);
    return Qnil;
  }
  
  return stmt;
}

/* call-seq:
 *   query(sql <, option>) -> Statement
 *   query(sql <, option>) { |row| block } -> nil
 *
 * ????????? SQL??? ????????? ????????? ?????? ???????????? ?????? ??? Statement ????????? ???????????????. 
 * ????????? Statement.execute??? ????????? ???????????? ?????? ????????? ????????? ??? ????????????.
 * 
 * option?????? Cubrid::INCLUDE_OID??? ??? ??? ?????????, ????????? prepare ???????????? ????????? ???????????????.
 *
 * block ???????????? Statement.fetch??? ???????????? ????????? ????????? ????????? ???????????? block??? ??????????????????. 
 * block??? SQL ?????? ????????? ????????? ?????? row ????????? ????????? ???????????????.
 * block??? ????????? Statement ????????? ??? ?????? ???????????? ????????????.
 *
 *  con = Cubrid.connect('demodb')
 *  stmt = con.query('SELECT * FROM db_user')
 *  while row = stmt.fetch 
 *    print row
 *  end
 *  stmt.close
 *  con.close
 * 
 *  stmt = con.query('SELECT * FROM db_user') { |row|
 *    print row
 *  }
 *  con.close
 */
VALUE 
cubrid_conn_query(int argc, VALUE* argv, VALUE self)
{
  VALUE stmt;
  Connection *con;

  GET_CONN_STRUCT(self, con);

  stmt = cubrid_conn_prepare_internal(argc, argv, self);
  cubrid_stmt_execute(0, NULL, stmt);
  
  if (rb_block_given_p()) {
    VALUE row;
    
    while(1) {
      row = cubrid_stmt_fetch(stmt);
      if (NIL_P(row)) {
        break;
      }
      rb_yield(row);
    }
    
    cubrid_stmt_close(stmt);
    return Qnil;
  }

  return stmt;
}

VALUE
cubrid_conn_end_tran(Connection *con, int type)
{
  int res;
  T_CCI_ERROR error;

  CHECK_CONNECTION(con, Qnil);

  res = cci_end_tran(con->handle, type, &error);
  if (res < 0){
    cubrid_handle_error(res, &error);
  }

  return Qnil;
}

/* call-seq:
 *   commit() -> nil
 * 
 * ??????????????? commit?????? ???????????????. 
 * ??????????????? ???????????? ??? ????????? ?????? ????????? ?????? Statement ????????? ?????? close ?????????.
 *
 */
VALUE
cubrid_conn_commit(VALUE self)
{
  Connection *con;

  GET_CONN_STRUCT(self, con);
  cubrid_conn_end_tran(con, CCI_TRAN_COMMIT);
  
  return Qnil;
}

/* call-seq:
 *   rollback() -> nil
 * 
 * ??????????????? rollback?????? ???????????????. 
 * ??????????????? ???????????? ??? ????????? ?????? ????????? ?????? Statement ????????? ?????? close ?????????.
 *
 */
VALUE
cubrid_conn_rollback(VALUE self)
{
  Connection *con;

  GET_CONN_STRUCT(self, con);
  cubrid_conn_end_tran(con, CCI_TRAN_ROLLBACK);
  
  return Qnil;
}

/* call-seq:
 *   auto_commit? -> true or false
 *
 * Connection??? auto commit ???????????? ???????????? ???????????????. 
 * Connection??? ??????????????? auto commit ????????? ?????????, auto_commit= ???????????? auto commit ????????? ????????? ??? ????????????.
 *
 */
VALUE
cubrid_conn_get_auto_commit(VALUE self)
{
  Connection *con;

  GET_CONN_STRUCT(self, con);
  CHECK_CONNECTION(con, Qnil);

  return con->auto_commit;
}

/* call-seq:
 *   auto_commit= true or false -> nil
 *
 * Connection??? auto commit ????????? ???????????????.
 * auto commit??? true??? ???????????? Statement.execute??? ????????? ?????? ??? ?????? commit??? ???????????????. 
 *
 */
VALUE
cubrid_conn_set_auto_commit(VALUE self, VALUE auto_commit)
{
  Connection *con;

  GET_CONN_STRUCT(self, con);
  CHECK_CONNECTION(con, self);

  con->auto_commit = auto_commit;
  return Qnil;
}

/* call-seq:
 *   to_s() -> string
 *
 * Connection??? ?????? ?????? ????????? ???????????? ???????????????.
 */
VALUE
cubrid_conn_to_s(VALUE self)
{
  char buf[MAX_STR_LEN];
  Connection *con;

  GET_CONN_STRUCT(self, con);
  sprintf(buf, "host: %s, port: %d, db: %s, user: %s", con->host, con->port, con->db, con->user);

  return rb_str_new2(buf);
}

/* call-seq:
 *   glo_new(classname <, filename>) -> Oid
 *
 * ????????? GLO ????????? ???????????? Oid??? ???????????????.
 * CUBRID??? ???????????? ???????????? ????????? ??? ????????? GLO??? ???????????????. GLO ????????? OID??? ?????? ????????? ??? ????????????.
 *
 * filename??? ???????????? ?????? ????????? ???????????? ????????????????????? ???????????????. ???????????? ????????? ??? GLO ????????? ???????????????.
 * 
 *  con = Cubrid.connect('subway')
 *  con.query('create table attachfile under glo (name string)')
 *  con.commit
 *
 *  glo = con.glo_new('attachfile', 'pic.jpg')
 *  glo.glo_size  #=> 1234
 *
 *  glo = con.glo_new('attachfile')
 *  glo.glo_size  #=> 0
 *  glo.glo_save('pic.jpg')
 *  glo.glo_size  #=> 1234
 */
VALUE
cubrid_conn_glo_new(VALUE self, VALUE table, VALUE file)
{
  char oid_str[MAX_STR_LEN], *table_name, *file_name;
  int res;
  T_CCI_ERROR error;
  Connection *con;

  GET_CONN_STRUCT(self, con);
  CHECK_CONNECTION(con, Qnil);

  if (NIL_P(table)) {
    rb_raise(rb_eArgError, "class name is required.");
    return Qnil;
  }
  table_name = StringValueCStr(table);
  
  if (NIL_P(file)) {
    file_name = NULL;
  } else {
    file_name = StringValueCStr(file);
  }
   
  res = cci_glo_new(con->handle, table_name, file_name, oid_str, &error);
  if (res < 0) {
    cubrid_handle_error(res, &error);
    return Qnil;
  }

  return cubrid_oid_new(con, oid_str);
}

/* call-seq:
 *   server_version() -> string
 *
 * ????????? ????????? ????????? ???????????? ???????????????.
 */
VALUE
cubrid_conn_server_version(VALUE self)
{
  char ver_str[MAX_STR_LEN];
  int res;
  Connection *con;

  GET_CONN_STRUCT(self, con);
  CHECK_CONNECTION(con, Qnil);

  res = cci_get_db_version(con->handle, ver_str, MAX_STR_LEN);
  if (res < 0) {
    cubrid_handle_error(res, NULL);
    return Qnil;
  }

  return rb_str_new2(ver_str);
}

