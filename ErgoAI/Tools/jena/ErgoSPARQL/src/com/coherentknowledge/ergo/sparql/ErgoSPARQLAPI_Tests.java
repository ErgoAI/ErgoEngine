/*
 * Copyright 2015-2016, Coherent Knowledge Systems.
 *
 * All rights reserved.
 */
package com.coherentknowledge.ergo.sparql;

import java.util.ArrayList;

/**
 * The ErgoSPARQLAPI_Tests class is used to test the ErgoSPARQLAPI class
 * methods.
 * 
 * @author Paul Fodor
 */
public class ErgoSPARQLAPI_Tests {

    /**
     * Constants used for testing.
     */
    public static final String SELECT_QUERY_SAMPLE = "SELECT * WHERE {?x ?r ?y} LIMIT 10";
    public static final String SELECT_COUNT_QUERY_SAMPLE = "SELECT (COUNT(*) AS ?no) { ?s ?p ?o }";
    public static final String CONSTRUCT_QUERY_SAMPLE = "CONSTRUCT { <http://example3.org/person> ?r ?y } WHERE {?x ?r ?y}  LIMIT 10";
    public static final String ASK_QUERY_SAMPLE = "ASK {?x ?r ?y}";
    public static final String DESCRIBE_QUERY_SAMPLE = "DESCRIBE ?y WHERE {?x ?r ?y} LIMIT 1";
    public static final String INSERT_QUERY_SAMPLE = "PREFIX dc: <http://purl.org/dc/elements/1.1/> INSERT DATA { <http://example/paul> dc:title \"A new book\" ; dc:creator \"A.N.Other\" . }";
    public static final String DELETE_QUERY_SAMPLE = "PREFIX dc: <http://purl.org/dc/elements/1.1/> DELETE DATA { <http://example/paul> dc:title \"A new book\" ; dc:creator \"A.N.Other\" . }";

    private static String answerFile = ".sparql-test-answers";

    /**
     * Main method used for testing.
     * 
     * @param args
     */
    public static void main(String[] args) {
        boolean dbpediaOpen = true;
        boolean dbpediaSparqlSelect = true;
        boolean dbpediaSparqlSelectAllCount = false;
        boolean dbpediaSparqlConstruct = true;
        boolean dbpediaSparqlAsk = true;
        boolean dbpediaSparqlDescribe = true;

        boolean coherentSparqlOpen = true;
        boolean coherentSparqlSelect = true;
        boolean coherentSparqlSelectAllCount = true;
        boolean coherentSparqlConstruct = true;
        boolean coherentSparqlAsk = true;
        boolean coherentSparqlDescribe = true;
        boolean coherentSparqlUpdate = true;

        try {
            // - DBPEDIA tests
            if (dbpediaOpen) {
                String targetService = "http://dbpedia.org/sparql";
                String username = "";
                String password = "";

                // run sparqlOpen with a fixed connectionId
                String connectionIdDbpedia = "DBPEDIAConnectionID";
                ErgoSPARQLAPI.sparqlOpen("query", connectionIdDbpedia, targetService,
                                         username, password);
                System.out.println("DBpedia SPARQL connection opened");
                System.out.println("\n-------------------------------------");

                // TEST TO OPEN THE CONNECTION AGAIN
                try {
                    ErgoSPARQLAPI.sparqlOpen("query", connectionIdDbpedia,
                                             targetService, username, password);
                } catch (Exception e) {
                    System.out.println("*** Testing SPARQL open exception:");
                    //e.printStackTrace();
                    System.out.println("*** End ErgoSPARQL exception");
                    System.out
                        .println("\n-------------------------------------");
                }

                // run sparqlOpen with Exception
                try {
                    ErgoSPARQLAPI.sparqlOpen("query", "IncorrectConnection",
                                             "http://coherentknowledge.com/nosparql", username,
                                             password);
                } catch (Exception e) {
                    System.out.println("*** Testing SPARQL open exception:");
                    //e.printStackTrace();
                    System.out.println("*** End ErgoSPARQL exception");
                    System.out
                        .println("\n-------------------------------------");
                }

                // run sparqlSelect
                if (dbpediaSparqlSelect) {
                    System.out.println("\nStart sparqlSelect:");
                    ErgoSPARQLAPI.sparqlSelect(answerFile,
                                      connectionIdDbpedia,
                                      SELECT_QUERY_SAMPLE);
                    /*
                    for (String[] oneTuple : selectResult) {
                        System.out.print("[");
                        for (String element : oneTuple)
                            System.out.print(element + " ");
                        System.out.println("]");
                    }
                    */
                    System.out.println("Done sparqlSelect!");
                    System.out
                        .println("\n-------------------------------------");
                }

                // run sparqlOpen with Exception
                try {
                    ErgoSPARQLAPI.sparqlSelect(answerFile,
                                               connectionIdDbpedia,
                                               "not a sparql select");
                } catch (Exception e) {
                    System.out.println("*** Testing SPARQL open exception:");
                    //e.printStackTrace();
                    System.out.println("*** End ErgoSPARQL exception");
                    System.out
                        .println("\n-------------------------------------");
                }

                // run sparqlSelect with Exception connectionID
                try {
                    ErgoSPARQLAPI.sparqlSelect(answerFile,
                                               "NoConnection",
                                               SELECT_QUERY_SAMPLE);
                    System.out
                        .println("*** Testing SPARQL select exception: invalid connectionID");
                    System.out
                        .println("*** This point should not be reached!!!!!!!!!!!!!!!!");
                } catch (Exception e) {
                    System.out
                        .println("*** Testing SPARQL select exception: invalid connectionID");
                    //e.printStackTrace();
                    System.out.println("*** End ErgoSPARQL exception");
                    System.out
                        .println("\n-------------------------------------");
                }

                // run sparqlSelect all count query
                if (dbpediaSparqlSelectAllCount) {
                    System.out.println("Start sparqlSelectAll count:");
                    ErgoSPARQLAPI.sparqlSelect(answerFile,
                                               connectionIdDbpedia,
                                               SELECT_COUNT_QUERY_SAMPLE);
                    /*
                    for (String[] oneTuple : selectCountResult) {
                        System.out.print("[");
                        for (String element : oneTuple)
                            System.out.print(element + " ");
                        System.out.println("]");
                    }
                    */
                    System.out.println("Done sparqlSelectAll count!");
                    System.out
                        .println("\n-------------------------------------");
                }

                // run sparqlConstruct
                if (dbpediaSparqlConstruct) {
                    System.out.println("Start sparqlConstruct:");
                    ErgoSPARQLAPI.sparqlConstruct(answerFile,
                                                  connectionIdDbpedia,
                                                  CONSTRUCT_QUERY_SAMPLE);
                    /*
                    for (String[] oneTuple : constructResult) {
                        System.out.print("[");
                        for (String element : oneTuple)
                            System.out.print(element + " ");
                        System.out.println("]");
                    }
                    */
                    System.out.println("Done sparqlConstruct!");
                    System.out
                        .println("\n-------------------------------------");
                }

                // run sparqlAsk
                if (dbpediaSparqlAsk) {
                    System.out.println("Start sparqlAsk:");
                    boolean askResult = ErgoSPARQLAPI.sparqlAsk(
                                                                connectionIdDbpedia, ASK_QUERY_SAMPLE);
                    System.out.println("askResult=" + askResult);
                    System.out.println("Done sparqlAsk!");
                    System.out
                        .println("\n-------------------------------------");
                }

                // run sparqlDescribe
                if (dbpediaSparqlDescribe) {
                    System.out.println("Start sparqlDescribe:");
                    ErgoSPARQLAPI.sparqlDescribe(answerFile,
                                                 connectionIdDbpedia,
                                                 DESCRIBE_QUERY_SAMPLE);
                    /*
                    for (String[] oneTuple : describeResult) {
                        System.out.print("[");
                        for (String element : oneTuple)
                            System.out.print(element + " ");
                        System.out.println("]");
                    }
                    */
                    System.out.println("Done sparqlDescribe!");
                    System.out
                        .println("\n-------------------------------------");
                }

                // run sparqlClose
                System.out.println("Dbpedia sparqlClose: "
                                   + ErgoSPARQLAPI.sparqlClose(connectionIdDbpedia));
                System.out.println("Dbpedia sparqlClose: "
                                   + ErgoSPARQLAPI.sparqlClose(connectionIdDbpedia));

                System.out.println("**************************************************************");
            }

            // --------------------------------------------------------------
            // Query our SPARQL triple store:
            // http://54.174.13.248:3030/dataset.html?tab=query&ds=/ds
            // run a sparqlOpen
            if (coherentSparqlOpen) {
                String targetService = "http://localhost:3030/ds/query";
                String username = "coherent";
                String password = "pass4coherent";

                // run sparqlOpen
                String connectionIdCoherent = "CoherentConnectionId";
                System.out.println("Opening Coherent Systems (http://localhost:3030/ds) sparqlOpen connectionID: "
                                   + connectionIdCoherent + " for querying.");
                ErgoSPARQLAPI.sparqlOpen("query", connectionIdCoherent, targetService,
                                         username, password);
                System.out.println("Connected Coherent Systems (http://localhost:3030/ds) sparqlOpen connectionID: "
                             + connectionIdCoherent);
                System.out.println("\n-------------------------------------");

                // run sparqlSelect
                if (coherentSparqlSelect) {
                    System.out.println("Start sparqlSelect:");
                    ErgoSPARQLAPI.sparqlSelect(answerFile,
                                               connectionIdCoherent,
                                               SELECT_QUERY_SAMPLE);
                    /*
                    for (String[] oneTuple : selectResult) {
                        System.out.print("[");
                        for (String element : oneTuple)
                            System.out.print(element + " ");
                        System.out.println("]");
                    }
                    */
                    System.out.println("Done sparqlSelect!");
                    System.out.println("\n-------------------------------------");
                }

                // run sparqlSelect count
                if (coherentSparqlSelectAllCount) {
                    ErgoSPARQLAPI.sparqlSelect(answerFile,
                                               connectionIdCoherent,
                                               SELECT_COUNT_QUERY_SAMPLE);
                    /*
                    for (String[] oneTuple : selectResult) {
                        System.out.print("[");
                        for (String element : oneTuple)
                            System.out.print(element + " ");
                        System.out.println("]");
                    }
                    */
                    System.out.println("Done sparqlSelect!");
                    System.out.println("Start sparqlSelectAll count:");
                    System.out
                        .println("\n-------------------------------------");
                }

                // run a SPARQL CONSTRUCT query
                if (coherentSparqlConstruct) {
                    System.out.println("Start sparqlConstruct:");
                    ErgoSPARQLAPI.sparqlConstruct(answerFile,
                                                  connectionIdCoherent,
                                                  CONSTRUCT_QUERY_SAMPLE);
                    /*
                    for (String[] oneTuple : constructResult) {
                        System.out.print("[");
                        for (String element : oneTuple)
                            System.out.print(element + " ");
                        System.out.println("]");
                    }
                    */
                    System.out.println("Done sparqlConstruct!");
                    System.out
                        .println("\n-------------------------------------");
                }

                // run a SPARQL ASK query
                if (coherentSparqlAsk) {
                    System.out.println("\nStart sparqlAsk:");
                    boolean askResult = ErgoSPARQLAPI.sparqlAsk(
                                                                connectionIdCoherent, ASK_QUERY_SAMPLE);
                    System.out.println("askResult=" + askResult);
                    System.out.println("Done sparqlAsk!");
                    System.out
                        .println("\n-------------------------------------");
                }

                // run a SPARQL DESCRIBE query
                if (coherentSparqlDescribe) {
                    System.out.println("Start sparqlDescribe:");
                    ErgoSPARQLAPI.sparqlDescribe(answerFile,
                                                 connectionIdCoherent,
                                                 DESCRIBE_QUERY_SAMPLE);
                    /*
                    for (String[] oneTuple : describeResult) {
                        System.out.print("[");
                        for (String element : oneTuple)
                            System.out.print(element + " ");
                        System.out.println("]");
                    }
                    */
                    System.out.println("Done sparqlDescribe!");
                    System.out
                        .println("\n-------------------------------------");
                }

                // run an insert and a delete
                if (coherentSparqlUpdate) {
					
                    String updateTargetService = "http://localhost:3030/ds/update";
                    // run sparqlOpen
                    String coherentUpdateConnectionId = "CoherentUpdateConnectionId";
                    System.out.println("Opening Coherent Systems (http://localhost:3030/ds) sparqlOpen connectionID: "
                                       + connectionIdCoherent + " for updating.");
                    ErgoSPARQLAPI.sparqlOpen("update", coherentUpdateConnectionId, targetService,
                                             username, password);
					
                    System.out.println("Start INSERT with sparqlUpdate:");
                    boolean insertResult =
                        ErgoSPARQLAPI.sparqlUpdate(
                                                   coherentUpdateConnectionId,
                                                   INSERT_QUERY_SAMPLE);
                    System.out.println("Done INSERT with sparqlUpdate: "
                                       + insertResult);

                    // see the result after insert
                    System.out.println("\nStart sparqlSelectAll:");
                    ErgoSPARQLAPI.sparqlSelect(answerFile,
                                               connectionIdCoherent,
                                               SELECT_QUERY_SAMPLE);
                    /*
                    for (String[] oneTuple : selectAllResult) {
                        System.out.print("[");
                        for (String element : oneTuple)
                            System.out.print(element + " ");
                        System.out.println("]");
                    }
                    */
                    System.out.println("Done sparqlSelectAll!");

                    // delete
                    System.out.println("\nStart DELETE with sparqlUpdate:");
                    boolean deleteResult = ErgoSPARQLAPI.sparqlUpdate(
                                                                      coherentUpdateConnectionId, DELETE_QUERY_SAMPLE);
                    System.out.println("Done DELETE with sparqlUpdate: "
                                       + deleteResult);

                    // see the result after delete
                    System.out.println("\nStart sparqlSelectAll:");
                    ErgoSPARQLAPI.sparqlSelect(answerFile,
                                               connectionIdCoherent,
                                               SELECT_QUERY_SAMPLE);
                    /*
                    for (String[] oneTuple : selectAllResult) {
                        System.out.print("[");
                        for (String element : oneTuple)
                            System.out.print(element + " ");
                        System.out.println("]");
                    }
                    */
                    System.out.println("Done sparqlSelectAll!");
                    System.out
                        .println("\n-------------------------------------");
                }

                // run sparqlClose
                System.out.println("CoherentKnowledge sparqlClose: "
                                   + ErgoSPARQLAPI.sparqlClose(connectionIdCoherent));
                System.out.println("CoherentKnowledge sparqlClose: "
                                   + ErgoSPARQLAPI.sparqlClose(connectionIdCoherent));

            }
        } catch (Exception e) {
            System.out.println("Exception!!!!!!!!!!!!!!!!!!!!!!!!!!!:");
            //e.printStackTrace();
            System.out.println("\n-------------------------------------");
        }

        System.out.println("\nDone ErgoSPARQLAPI tests!!!!!!!!!!!!!!!!!!");
    }
}
