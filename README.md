# UniversalMigrate

Have you ever needed a simple migration tool but only found large and complex software?
It happened to me, and that's why I built this simple tool, which can easily control how your software versions

You should only informate your server infos on file 'data/server.xml' and start your migrations

## migrate new 
> * creates an new migration file template on data/migrations.

## migrate update 
> * use the files on 'data/migrations' to update your database.

## migrate list 
> * list all migrations that was loaded on your server.

## migrate remove id_migrate 
> * run downgrade and remove an migration.

## migrate reset 

> * run downgrade and remove to all migrations 

### Create the migration template, edit the migration and downgrade files, then run migrate update to send it to database server.

## As i said. Its so simple!

###
Installation

### You need three dependencies

* `make` for the Makefile compilation
* `mysqlclient` for the mysql connector 
* `libxml` for the configuration reader 


Calisto – jovictor210@gmail.com

## Contributing
1. Fork (<https://github.com/stdmedoth/UniversalMigrate//fork>)
2. Create your branch (`git checkout -b feature/something`)
3. Commit changes (`git commit -am 'add something'`)
4. Upload as a branch (`git push origin feature/something`)
5. Create a pull request
