/*
 * Copyright 2015-2016, Coherent Knowledge Systems.
 *
 * All rights reserved.
 */
package com.coherentknowledge.ergo.sparql;

import java.util.HashMap;
import java.util.ArrayList;
import java.util.List;
import java.util.Iterator;
import java.util.Scanner;

import java.io.BufferedWriter;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.StandardOpenOption;

import org.apache.jena.atlas.web.auth.HttpAuthenticator;
import org.apache.jena.atlas.web.auth.SimpleAuthenticator;
import org.apache.log4j.Logger;

import com.hp.hpl.jena.datatypes.RDFDatatype;
import com.hp.hpl.jena.query.QueryExecution;
import com.hp.hpl.jena.query.QueryExecutionFactory;
import com.hp.hpl.jena.query.QueryFactory;
import com.hp.hpl.jena.query.QuerySolution;
import com.hp.hpl.jena.query.Query;
import com.hp.hpl.jena.query.Syntax;
import com.hp.hpl.jena.query.ResultSet;
import com.hp.hpl.jena.rdf.model.Model;
import com.hp.hpl.jena.rdf.model.Literal;
import com.hp.hpl.jena.rdf.model.Property;
import com.hp.hpl.jena.rdf.model.RDFNode;
import com.hp.hpl.jena.rdf.model.Resource;
import com.hp.hpl.jena.rdf.model.Statement;
import com.hp.hpl.jena.rdf.model.StmtIterator;
import com.hp.hpl.jena.sparql.core.Var;
import com.hp.hpl.jena.update.UpdateExecutionFactory;
import com.hp.hpl.jena.update.UpdateFactory;
import com.hp.hpl.jena.update.UpdateProcessor;

/**
 * The ErgoSPARQLAPI class is used by the ErgoStudio API to query a SPARQL
 * endpoint.
 * 
 * @author Paul Fodor
 */
public class ErgoSPARQLAPI {

    private static Logger logger = Logger.getLogger("ErgoSPARQL");

    /**
     * cache for storing targetService
     */
    private static HashMap<String, String> cache_services = new HashMap<String, String>();
    private static HashMap<String, String> cached_connection_types = new HashMap<String, String>();

    /**
     * cache for storing authenticators
     */
    private static HashMap<String, HttpAuthenticator> cache_authenticators = new HashMap<String, HttpAuthenticator>();


    /*
     * Using constructor in order to initialize cache.
     */
    public ErgoSPARQLAPI() {
        // Logger.getRootLogger().setLevel(Level.OFF);
    }

    /**
     * Method to start a connection.
     * 
     * @param connectionID
     *            - a fixed connectionID (like dbpedia, coherentKnowledge, etc.)
     * @param targetService
     * @param username
     * @param password
     *
     * @return void
     */
    public static void sparqlOpen(String connectionType,
                                  String connectionId, String targetService,
                                  String username, String password)
        throws Exception
    {
        if(cache_services.containsKey(connectionId)){
            // throw exception that the connection is already open and 
            // ask the user to close the connection first
            throw new Exception("ErgoSPARQL(open): connection id is already in use; please close it and then reopen");
        }
        try {
            HttpAuthenticator authenticator =
                new SimpleAuthenticator(username, password.toCharArray());
            // Test simple query to verify the authenticator.
            if (connectionType.equals("query")) {
                QueryExecution query_execution =
                    QueryExecutionFactory.sparqlService(targetService,
                                                        "SELECT * WHERE {?x ?r ?y} LIMIT 1",
                                                        authenticator);
                query_execution.execSelect();
                query_execution.close();
            } else {
                UpdateProcessor update_execution =
                    UpdateExecutionFactory.createRemote(UpdateFactory.create("INSERT DATA { }"),
                                                        targetService,
                                                        authenticator);
                // UpdateProcessor has no close() method
                update_execution.execute();
            }
            // put the connection in the cache
            cache_services.put(connectionId, targetService);
            cached_connection_types.put(connectionId, connectionType);
            cache_authenticators.put(connectionId, authenticator);
        } catch (Exception e) {
            String error =
                "\nsparqlOpen error: " + "\n connectionId=" + "\""
                + connectionId + "\"" + "\n targetService=" + "\""
                + targetService + "\"" + "\n username=" + "\"" + username
                + "\"" + "\n password=" + "\"" + password + "\"";
            logger.error(error, e);
            throw new Exception("ErgoSPARQL(open): "
                                + connectionType + " connection to "
                                + targetService
                                + " cannot be established.\n\t\tPlease verify user credentials and that the above URL is a valid SPARQL "
                                + connectionType
                                + " endpoint");
        }
    }


    /**
     * Method to test if a connection is open and get connection type
     * 
     * @param connectionID
     *
     * @return connection type or null
     */
    public static String sparqlConnectionType(String connectionId) {
        if(cache_services.containsKey(connectionId)){
            if (cached_connection_types.get(connectionId).equals("update"))
                return "update";
            else
                return "query";
        } else
            return null;
    }


    /**
     * Method to test if a connection is open and get connection URL
     * 
     * @param connectionID
     *
     * @return connection URL or null
     */
    public static String sparqlConnectionURL(String connectionId) {
        if(cache_services.containsKey(connectionId)){
            return cache_services.get(connectionId);
        } else
            return null;
    }


    /**
     * Method to stop a connection.
     * 
     * @param connectionID
     *            - a fixed connectionID (like dbpedia, coherentKnowledge, etc.)
     *
     * @return boolean true if the connection exists; false if not
     */
    public static boolean sparqlClose(String connectionId) throws Exception
    {
        if (cache_services.get(connectionId) != null) {
            cache_services.remove(connectionId);
            cached_connection_types.remove(connectionId);
            cache_authenticators.remove(connectionId);
            return true;
        } else {
            // Multi-close: throw an exception because otherwise the
            // user does not know that he closed a SPARQL stream already
            throw new Exception("ErgoSPARQL(close): the connection " + connectionId + " does not exist or was closed");
        }
    }

    /**
     * Method for SPARQL SELECT that writes out all the results in ansFile
     * 
     * @param connectionId
     * @param query
     * 
     * @return void
     */
    public static void sparqlSelect(String ansFile,
                                    String connectionId,
                                    String query)
        throws Exception
    {
        BufferedWriter writer = makeAnswerFile("select",ansFile,query);

        try {
            QueryExecution query_execution =
                constructQueryExecution(connectionId, query);

            // Get all projection variables (in SELECT ... WHERE) from query
            Query queryObj = QueryFactory.create(query, Syntax.syntaxARQ);
            List<Var> vars = queryObj.getProjectVars();
            Iterator<Var> varsIterator = vars.iterator();
            ArrayList<String> varArrayList = new ArrayList<String>();

            while(varsIterator.hasNext()){
                String var = varsIterator.next().getVarName();
                varArrayList.add(var);
            }

            if (query_execution != null) {
                /*
                  NOTE:
                     execSelect() is a slow operation in Jena.
                     The actual select may be returned from a triple store in
                     a fraction of a second, but Jena will come back from
                     execSelect() (for 7-10K) answers only after 4-6 seconds.
                */
                ResultSet resultSet = query_execution.execSelect(); // slow
                while (resultSet.hasNext()) {
                    QuerySolution querySolution = resultSet.nextSolution();

                    // write out 1 solution as Prolog list
                    writer.write("[");
                    boolean start = true; // controls printing "," in the loop
                    for (String varName : varArrayList) {
                        String value = RDFNode2String(querySolution.get(varName));
                        if (start) start = false;
                        else writer.write(",");

                        writer.write(value);
                    }

                    writer.write("].\n");
                }

                query_execution.close();
                if (writer != null) writer.close();
            }
        } catch(ErgoSPARQLNoConnectionIDException e) {
            String error =
                "\nsparqlSelect error: " + "\n connectionId=" + "\""
                + connectionId + "\"" + "\n query=" + "\""
                + query + "\"";
            logger.error(error, e);
            throw new Exception("ErgoSPARQL(select): the connection " + connectionId + " does not exist or was closed");
        } catch (Exception e) {
            String error =
                "\nsparqlSelect error: " + "\n connectionId=" + "\""
                + connectionId + "\"" + "\n query=" + "\""
                + query + "\"";
            logger.error(error, e);

            wrongTypeSyntaxOrNoConnectionError("select",connectionId,query,e);
        }

        if (writer != null) writer.close();
    }


    /**
     * Method for SPARQL CONSTRUCT query. CONSTRUCT is an alternative SPARQL
     * result clause to SELECT. Instead of returning a table of result values,
     * CONSTRUCT returns an RDF graph. The result RDF graph is created by taking
     * the results of the equivalent SELECT query and filling in the values of
     * variables that occur in the CONSTRUCT query. Triples are not created in
     * the result graph for query patterns that involve an unbound variable.
     * 
     * @param connectionId
     * @param query
     * 
     * @return void
     */
    public static void sparqlConstruct(String ansFile,
                                       String connectionId,
                                       String query)
        throws Exception
    {
        BufferedWriter writer = makeAnswerFile("construct",ansFile,query);

        try {
            QueryExecution query_execution =
                constructQueryExecution(connectionId, query);
            if (query_execution != null) {
                Model constructModel = query_execution.execConstruct();
                StmtIterator modelIterator = constructModel.listStatements();
                writeSPOtriplesAsPrologLists(modelIterator,writer);

                query_execution.close();
                if (writer != null) writer.close();
            }
        } catch(ErgoSPARQLNoConnectionIDException e){
            String error =
                "\nsparqlConstruct error: " + "\n connectionId=" + "\""
                + connectionId + "\"" + "\n query=" + "\""
                + query + "\"";
            logger.error(error, e);
            throw new Exception("ErgoSPARQL(construct): the connection " + connectionId + " does not exist or was closed");
        } catch (Exception e) {
            String error =
                "\nsparqlConstruct error: " + "\n connectionId="
                + "\"" + connectionId + "\"" + "\n query=" + "\""
                + query + "\"";
            logger.error(error, e);

            wrongTypeSyntaxOrNoConnectionError("construct",connectionId,query,e);
        }

        if (writer != null) writer.close();
    }

    /**
     * Method for SPARQL ASK query.
     * 
     * @param connectionId
     * @param query
     * 
     * @return a boolean: true or false
     */
    public static boolean sparqlAsk(String connectionId, String query)
        throws Exception
    {
        try {
            QueryExecution query_execution =
                constructQueryExecution(connectionId, query);
            if (query_execution != null) {
                boolean result = query_execution.execAsk();
                query_execution.close();
                return result;
            }
        } catch(ErgoSPARQLNoConnectionIDException e){
            String error =
                "\nsparqlAsk error: " + "\n connectionId=" + "\""
                + connectionId + "\"" + "\n query=" + "\""
                + query + "\"";
            logger.error(error, e);
            throw new Exception("ErgoSPARQL(ask): the connection " + connectionId + " does not exist or was closed");
        } catch (Exception e) {
            String error =
                "sparqlAsk error: " + "\n connectionId=" + "\""
                + connectionId + "\"" + "\n query=" + "\"" + query
                + "\"";
            logger.error(error, e);

            wrongTypeSyntaxOrNoConnectionError("ask",connectionId,query,e);
        }
        return false;
    }

    /**
     * Method for SPARQL DESCRIBE query.
     * 
     * @param connectionId
     * @param query
     * 
     * @return void
     */
    public static void sparqlDescribe(String ansFile,
                                      String connectionId,
                                      String query)
        throws Exception
    {
        BufferedWriter writer = makeAnswerFile("describe",ansFile,query);

        try {
            QueryExecution query_execution =
                constructQueryExecution(connectionId, query);
            if (query_execution != null) {
                Model describeModel = query_execution.execDescribe();
                StmtIterator modelIterator = describeModel.listStatements();
                writeSPOtriplesAsPrologLists(modelIterator,writer);

                query_execution.close();
                if (writer != null) writer.close();
            }
        } catch(ErgoSPARQLNoConnectionIDException e){
            String error =
                "\nsparqlDescribe error: " + "\n connectionId=" + "\""
                + connectionId + "\"" + "\n query=" + "\""
                + query + "\"";
            logger.error(error, e);
            throw new Exception("ErgoSPARQL(describe): the connection " + connectionId + " does not exist or was closed");
        } catch (Exception e) {
            String error =
                "\nsparqlDescribe error: " + "\n connectionId="
                + "\"" + connectionId + "\"" + "\n query=" + "\""
                + query + "\"";
            logger.error(error, e);

            wrongTypeSyntaxOrNoConnectionError("describe",connectionId,query,e);
        }

        if (writer != null) writer.close();
    }


    /**
     * Method for SPARQL UPDATE.
     * 
     * @param connectionId
     * @param query
     * 
     * @return true for success; false for fail.
     */
    public static boolean sparqlUpdate(String connectionId, String query)
        throws Exception
    {
        try {
            String targetService = cache_services.get(connectionId);
            if (targetService == null)
                throw new ErgoSPARQLNoConnectionIDException(
                                                            "SPARQL endpoint for the given connection id not found");
            HttpAuthenticator authenticator =
                cache_authenticators.get(connectionId);
            if (targetService != null && authenticator != null) {
                UpdateProcessor upp =
                    UpdateExecutionFactory.createRemote(UpdateFactory.create(query),
                                                        targetService,
                                                        authenticator);
                upp.execute();
                return true;
            }
        } catch(ErgoSPARQLNoConnectionIDException e){
            String error =
                "\nsparqlUpdate error: " + "\n connectionId=" + "\""
                + connectionId + "\"" + "\n update=" + "\""
                + query + "\"";
            logger.error(error, e);
            throw new Exception("ErgoSPARQL(update): the connection " + connectionId + " does not exist or was closed");
        } catch (Exception e) {
            String error =
                "sparqlUpdate error: " + "\n connectionId=" + "\""
                + connectionId + "\"" + "\n update=" + "\""
                + query + "\"";
            logger.error(error, e);

            wrongTypeSyntaxOrNoConnectionError("update",connectionId,query,e);
        }
        return false;
    }

    /************************* Utilities ********************************/

    private static String RDFNode2String(RDFNode node) {
        // All items are put inside the '....' -- as Prolog atoms
        // Any existing single quotes are doubled with replace(...)

        // this usually happens when query return list has variables
        // that are not found in the body
        if (node == null) return "'NULL'(_)";

        if (node.isURIResource()) {
            return "'i\b" + node.toString().replace("'","''") + "'";
        } else if (node.isAnon()) { // blank nodes are nasty
            // internal representation of blank nodes to communicate with Ergo
            return "'\b\b\bRDFblanknode'";
        } else if (node.isLiteral()) {
            Literal literal = node.asLiteral();
            String lexical = literal.getLexicalForm().replace("'","''");
            String typeuri = literal.getDatatypeURI();

            // this enables the use of the fast type comparison using ==
            if (typeuri != null)
                typeuri = typeuri.intern();
            else
                // Literals that have no type: represent them as Ergo \string
                // s\b - internal representation of strings in Ergo
                return "'s\b" + lexical + "'";

            switch (typeuri) {
            case "http://www.w3.org/2001/XMLSchema#string":
                // optimization for strings (on the prolog side)
                return "'s\b" + lexical + "'";
            // optimization for numbers (on the prolog side)
            case "http://www.w3.org/2001/XMLSchema#integer":
            case "http://www.w3.org/2001/XMLSchema#long":
            case "http://www.w3.org/2001/XMLSchema#short":
            case "http://www.w3.org/2001/XMLSchema#positiveInteger":
            case "http://www.w3.org/2001/XMLSchema#nonPositiveInteger":
            case "http://www.w3.org/2001/XMLSchema#negativeInteger":
            case "http://www.w3.org/2001/XMLSchema#nonNegativeInteger":
            case "http://www.w3.org/2001/XMLSchema#decimal":
            case "http://www.w3.org/2001/XMLSchema#float":
            case "http://www.w3.org/2001/XMLSchema#double":
                return lexical;
            default:
                // types that are not numbers of strings
                // "i\b" - this prefix converts type name to
                // the internal representation of Ergo IRIs
                return "'\""+lexical+"\"^^''"+"i\b"+typeuri+"'''";
            }
        } else
            // s\b - internal representation of strings in Ergo
            return "'s\b" + node.toString().replace("'","''") + "'";
    }
    

    /**
     * Method to create a QueryExecution object. Used by the methods for SPARQL
     * SELECT, CONSTRUCT, ASK and DESCRIBE.
     * 
     * @param connectionId
     * @param query
     * 
     * @return a QueryExecution object or null if the service is not available
     */
    private static QueryExecution constructQueryExecution(String connectionId,
                                                          String query)
        throws Exception
    {
        try {
            String targetService = cache_services.get(connectionId);
            if (targetService == null)
                throw new ErgoSPARQLNoConnectionIDException("SPARQL endpoint connectionID not found");
            HttpAuthenticator authenticator =
                cache_authenticators.get(connectionId);
            if (targetService != null && authenticator != null) {
                QueryExecution query_executionModel =
                    QueryExecutionFactory.sparqlService(targetService,
                                                        query,
                                                        authenticator);
                return query_executionModel;
            }
        } catch (Exception e) {
            // throw the error again
            throw e;
        }
        return null;
    }


    /**
     * Method to write out SPO triples for Describe and Construct
     * 
     * @param resultModel
     * 
     * @return void
     */
    private static void writeSPOtriplesAsPrologLists(StmtIterator resultModel,
                                                     BufferedWriter writer)
        throws Exception
    {
        try {
            while (resultModel.hasNext()) {
                Statement stmt = resultModel.nextStatement();
                Resource subject = stmt.getSubject(); // get the subject
                Property predicate = stmt.getPredicate(); // get the predicate
                RDFNode object = stmt.getObject(); // get the object
                // RDFNode2String detects types (IRI, datatypes, blank nodes);
                // the rest are returned as "Literal"^^type
                String subjectString = RDFNode2String(subject);
                String predicateString = RDFNode2String(predicate);
                String objectString = RDFNode2String(object);

                if (subjectString != null) {
                    writer.write("[");
                    writer.write(subjectString + ",");
                    writer.write(predicateString + ",");
                    writer.write(objectString);
                    writer.write("].\n");
                }
            }
        } catch (Exception e) {
            // throw the error again
            throw e;
        }
    }

    private static BufferedWriter makeAnswerFile(String op, String ansFile, String query)
        throws Exception
    {
        Path ansFilePath = Paths.get(ansFile);
        BufferedWriter writer = null;
        try {
            writer = Files.newBufferedWriter(ansFilePath,
                                             StandardCharsets.UTF_8);
        } catch (Exception e) {
            throw new Exception("ErgoSPARQL("
                                + op
                                + "): cannot open temporary file "
                                + ansFile
                                + " for " + query);
        }
        return writer;
    }


    private static void wrongTypeSyntaxOrNoConnectionError(String operation,
                                                           String connectionId,
                                                           String query,
                                                           Exception e)
        throws Exception
    {
        Boolean wrongConnectionTypeQueryUpdate =
            // operation = query; connection type = update
            cached_connection_types.get(connectionId).equals("update")
            && !operation.equals("update");
        Boolean wrongConnectionTypeUpdateQuery =
            // operation = update; connection type = query
            !cached_connection_types.get(connectionId).equals("update")
            && operation.equals("update");

        String typeError = "";
        String serviceEndpoint = cache_services.get(connectionId);
        if (wrongConnectionTypeQueryUpdate)
            // executing query via an update endpint
            typeError = "a query via an update";
        if (wrongConnectionTypeUpdateQuery)
            // executing update via a query endpint
            typeError = "an update via a query";
        if (wrongConnectionTypeQueryUpdate || wrongConnectionTypeUpdateQuery)
            throw new Exception("ErgoSPARQL("
                                + operation
                                + "): trying to execute "
                                + typeError
                                + " endpoint;\n\t\tconnection = "
                                + connectionId
                                + " ("
                                + serviceEndpoint
                                + ");\n\t\tquery = "
                                + query);

        String errMessage = e.getMessage();
        if (errMessage == null) {
            throw new Exception("ErgoSPARQL("
                                + operation
                                + "): possible bug: "
                                + e
                                + "\n\tconnection = "
                                + connectionId
                                + " ("
                                + serviceEndpoint
                                + ");\n\tquery = "
                                + query);
        }
        else
            if (errMessage.matches(".*HttpHostConnectException.*Connection.*refused.*")) {
                // connection is down
                throw new Exception("ErgoSPARQL("
                                    + operation
                                    + "): SPARQL endpoint is down;\n\t\tconnection = "
                                    + connectionId
                                    + " ("
                                    + serviceEndpoint
                                    + ");\n\tquery = "
                                    + query);
            } else if (e.getClass().toString().matches(".*QueryParseException.*")) {
                // syntax error
                throw new Exception("ErgoSPARQL("
                                    + operation
                                    + "): syntax error;\n\tquery = "
                                    + query
                                    + "\n*** "
                                    + errMessage
                                    + "\n");
            } else {
                System.out.println("\n"+e);
                throw(e);
            }
    }
}

@SuppressWarnings("serial")
class ErgoSPARQLNoConnectionIDException extends Exception {
    ErgoSPARQLNoConnectionIDException(String m) {
        super(m);
    }
}
