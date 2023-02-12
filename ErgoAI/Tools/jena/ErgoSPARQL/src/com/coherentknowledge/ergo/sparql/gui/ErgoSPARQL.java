/*
 * Copyright 2015-2016, Coherent Knowledge Systems.
 *
 * All rights reserved.
 */
package com.coherentknowledge.ergo.sparql.gui;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Set;

import com.hp.hpl.jena.query.ParameterizedSparqlString;
import com.hp.hpl.jena.query.Query;
import com.hp.hpl.jena.query.QueryFactory;
import com.hp.hpl.jena.query.QuerySolution;
import com.hp.hpl.jena.query.QuerySolutionMap;
import com.hp.hpl.jena.query.ResultSet;
import com.hp.hpl.jena.query.Syntax;
import com.hp.hpl.jena.rdf.model.RDFNode;
import com.hp.hpl.jena.sparql.core.Var;
import com.hp.hpl.jena.sparql.engine.http.QueryEngineHTTP;

/**
 *
 * @author Paul Fodor
 */
public class ErgoSPARQL {
        
    // flag to select between fastload output format and Ergo output
    //  fastload = 1
    //  predicates = 2
    static int outputFormat = 1;

    // iris2links: xsd -> http://www.w3.org/2001/XMLSchema#, ... 
    public static ArrayList<String> irisArrayList;
    // iris and links
    public static HashMap<String, String> iris2links;

    /**
     * initializeIris
     */
    public static void initializeDefaultIris() {
        irisArrayList = new ArrayList<String>();
        iris2links = new HashMap<String, String>();
        irisArrayList.add("xsd");
        iris2links.put("xsd", "http://www.w3.org/2001/XMLSchema#");
        irisArrayList.add("rdf");
        iris2links.put("rdf", "http://www.w3.org/1999/02/22-rdf-syntax-ns#");
        irisArrayList.add("rdfs");
        iris2links.put("rdfs", "http://www.w3.org/2000/01/rdf-schema#");
        irisArrayList.add("owl");
        iris2links.put("owl", "http://www.w3.org/2002/07/owl#");        
    }
        
    /**
     * initializeIris for DBPedia 
     *   part of the basic test for SPARQL endpoints
     */
    public static void initializeIrisDBPedia() {
        initializeDefaultIris();
        // DBpedia IRIs
        irisArrayList.add("dbpedia");
        iris2links.put("dbpedia", "http://dbpedia.org/resource/");
        irisArrayList.add("dbpedia-owl");
        iris2links.put("dbpedia-owl", "http://dbpedia.org/ontology/");
        irisArrayList.add("dbprop");
        iris2links.put("dbprop", "http://dbpedia.org/property/");
        irisArrayList.add("foaf");
        iris2links.put("foaf", "http://xmlns.com/foaf/0.1/");
        irisArrayList.add("example");
        iris2links.put("example", "http://example/");
        irisArrayList.add("dcelements");
        iris2links.put("dcelements", "http://purl.org/dc/elements/1.1/");
    }

    /**
     * setIris
     *   the argument is a string with "PREFIX iri: <http://iriLink>" lines
     */
    public static void setIris(String irisText) {
        irisArrayList = new ArrayList<String>();
        iris2links = new HashMap<String, String>();
        String[] iris = irisText.split("\n");
        for(int i=0; i<iris.length; i++){
            String[] oneIri = iris[i].split(": ");
            try{
                if(!oneIri[0].trim().equals("") && !oneIri[0].toUpperCase().contains("PREFIX "))
                    throw new IllegalArgumentException("Incorrect iri");
                String iri;
                if(oneIri[0].trim().equalsIgnoreCase("PREFIX")){
                    iri = ""; // default IRI
                }else{
                    iri = oneIri[0].trim().substring(oneIri[0].indexOf(' ')+1).trim();
                    for(char c:iri.toCharArray())
                        if(!Character.isLetter(c) && !Character.isDigit(c) && c!='_' && c!='-' && c!='.')
                            throw new IllegalArgumentException("Incorrect iri");
                }
                String link = oneIri[1].trim().substring(1,oneIri[1].length()-1);
                irisArrayList.add(iri);
                iris2links.put(iri, link);
            }catch(Exception e){
                e.printStackTrace();
                System.out.println("Incorrect IRI: " + iris[i]);
                throw new IllegalArgumentException("Incorrect IRI preffixes");
            }
        }
    }
        
    /**
     * getIrisText
     * @return a string that we will put in the UI
     */
    public static String getIrisText() {
        String irisTextAreaString = "";
        String iriLink;
        for (String iri : irisArrayList) {
            iriLink = iris2links.get(iri);
            irisTextAreaString += "PREFIX " + iri + ": <" + iriLink + ">\n";
        }
                
        return irisTextAreaString;
    }

    // fastload does not allow dash in IRIs, so we replace them with _dash_
    public static String outputIRI(String iri) {
        return iri.replaceAll("-", "_dash_").replaceAll("\\.", "_dot_");
    }
        
    /**
     * replaceLinks2IRIs
     *  replace links with IRIs in the result
     * @param value that may contain an IRI link
     * @return value replaced with IRI
     */
    public static String replaceLinks2IRIs(String value) {
        Set<String> iris = iris2links.keySet();
        String iriLink;
        String returnValue;
        for (String iri : iris) {
            iriLink = iris2links.get(iri);
            if (value.contains(iriLink)) {
                int endPos = value.indexOf(iriLink) + iriLink.length();
                // output format
                if(outputFormat == 1){ // fastload
                    String subValue = value.substring(endPos);
                    //if(subValue.startsWith("_"))
                    //  subValue = "'" + subValue + "'";
                    returnValue = "__fCURI__(" + outputIRI(iri) + "," + subValue + ")";
                    return returnValue;
                }else if(outputFormat == 2){
                    returnValue = iri + "#" + value.substring(endPos);
                    return returnValue;
                }       
            }
        }
        // if no IRI was found, return the same string
        return value;
    }

    /**
     * parseSolutions
     */
    public static ArrayList<String> parseSolutions(String[] args) {
        ArrayList<String> returnValueArrayList = new ArrayList<String>();
        for(int i=0; i<args.length; i++){
            args[i] = args[i].trim();
            String elem = "";
            // see if the first argument is a link
            if(args[i].equals("")){
                elem = "'NULL'()"; // the empty atom in Ergo
            } else if(args[i].startsWith("http://")){
                elem = replaceLinks2IRIs(args[i]);
                // Put single quotes around the link only if a prefix is not created 
                if(args[i].equals(elem) && !elem.startsWith("__fCURI__")) 
                    elem = "'" + elem + "'";
            } else {
                // check if it is not a default iri
                boolean aDefaultIRI = false;
                String defaultIRI = "";
                if(args[i].contains(":")){
                    String[] defaultIRIArray = args[i].split(":");
                    if(irisArrayList.contains(defaultIRIArray[0])){
                        aDefaultIRI = true;
                        if(outputFormat == 1){ // fastload
                            defaultIRI = "__fCURI__(" + outputIRI(defaultIRIArray[0]) + "," + defaultIRIArray[1] + ")";
                        }else { //if(outputFormat == 2)
                            defaultIRI = defaultIRIArray[0] + "#" + defaultIRIArray[1];
                        }       
                    }
                }
                if(aDefaultIRI)
                    elem = defaultIRI;
                else{ // If the first argument is a string 
                    elem = args[i];
                    // eliminate the type: ^^type in the remainder
                    if(elem.indexOf("\"^^") >= 0)
                        elem = elem.substring(1, elem.indexOf("\"^") + 1);
                    //replace the single quotes in the String with escaped single quotes
                    elem = elem.replaceAll("'","'NULL'()");
                    // replace Unicode characters with prolog ISO (4 hex digits only)
                    if(elem.indexOf("\\u")>=0){
                        int pos = elem.indexOf("\\u");
                        while(pos>0 && elem.length() >= pos+5){
                            elem = elem.substring(0,pos+6) + "\\" + elem.substring(pos+6);
                            pos = elem.indexOf("\\u",pos+5);
                        }
                    }
                    elem = "'" + elem + "'";
                }       
            }
            // if the elem starts with _, but it is not an IRI, then put the elem in single quotes
            if(elem.startsWith("_") && !elem.startsWith("__fCURI__"))
                elem = "'" + elem + "'";
            returnValueArrayList.add(elem);
        }

        return returnValueArrayList;
    }

    // query Endpoint
    public static String queryEndpoint(String sparqlEndpointString, 
                                       String userString, String passwordString, 
                                       String irisString, String queryString, String predicateString,
                                       File outputFile) throws IOException {
        // add the IRIs in the .ergo file prefix
        String ergoPrefix = "";
                
        if(outputFormat == 1){
            ergoPrefix = "";
            /*
              "#mode save\n"
              + "#mode nostring   \"\\!#'\"\n"
              + "#define __fCURI__(X,Y)   'i\\x08\\#1#2'\n"
              + "#mode restore\n\n";
            */
            // add the IRIs to the ergoPrefix
            for (String iri : irisArrayList) {
                String link = iris2links.get(iri);
                ergoPrefix += "#deffast   " + outputIRI(iri) + " " + link + "\n";
            }
            // add a comment in the .ergo file prefix before the translation
            ergoPrefix += "\n% imported OWL axioms\n";
        } else {
            ergoPrefix = "/* Built-in prefixes used:\n"
                + "xsd 'http://www.w3.org/2001/XMLSchema#'\n"
                + "rdf 'http://www.w3.org/1999/02/22-rdf-syntax-ns#'\n"
                + "rdfs 'http://www.w3.org/2000/01/rdf-schema#'\n"
                + "owl 'http://www.w3.org/2002/07/owl#'\n" + "*/\n\n"
                + "/* Other prefixes used:  */\n";
            // add the other IRIs to the ergoPrefix
            for (String iri : irisArrayList) {
                String link = iris2links.get(iri);
                if (!iri.equals("xsd") 
                    && !iri.equals("rdf") 
                    && !iri.equals("rdfs") 
                    && !iri.equals("owl"))
                    ergoPrefix += ":- iriprefix{" + iri + " = '" + link + "'}.\n";
            }
            // add a comment in the .ergo file prefix before the translation
            ergoPrefix += "\n/* imported OWL axioms -- these are in the form of fact triples*/\n";
        }

        // collect the output for the return to the TextArea
        String ergoProgram = ergoPrefix;
        int stopCounter = 0; // stop the collection of the text after 100 lines

        Query query = QueryFactory.create(irisString + "\n" + queryString, Syntax.syntaxARQ);
        ParameterizedSparqlString parameterizedSparqlString = new ParameterizedSparqlString(query.toString());
        QueryEngineHTTP httpQuery = new QueryEngineHTTP(sparqlEndpointString, parameterizedSparqlString.asQuery());
                
        if(!userString.isEmpty())
            httpQuery.setBasicAuthentication(userString, passwordString.toCharArray());
                
        FileWriter ergoOutputFileWriter = new FileWriter(outputFile);
        ergoOutputFileWriter.write(ergoPrefix);
        String ergoStatement;
                
        // collect output variable names from the query
        List<Var> vars = query.getProjectVars(); 
        Iterator<Var> varsIterator = vars.iterator();
        ArrayList<String> varArrayList = new ArrayList<String>();
        while(varsIterator.hasNext()){
            String var = varsIterator.next().getVarName();
            varArrayList.add(var);
        }

        // collect results
        ResultSet results = httpQuery.execSelect();
        ArrayList<String> values = new ArrayList<String>();
        String[] solutionArray = new String[varArrayList.size()];
        QuerySolution solution;
        RDFNode solutionRDFNode;
                
        while (results.hasNext()) {
            solution = results.next();
            // collect results for each variable in the query result
            for(int i=0; i<varArrayList.size(); i++){
                solutionRDFNode = solution.get(varArrayList.get(i));
                if(solutionRDFNode!=null)
                    solutionArray[i] = solution.get(varArrayList.get(i)).toString();
                else // if the variable value is not in the array (e.g., SPARQL OPTIONAL)
                    solutionArray[i] = "";
            }
                        
            ergoStatement = "";
                        
            values = parseSolutions(solutionArray);
            ergoStatement = predicateString + "(";
            for(int i=0; i<solutionArray.length; i++){
                ergoStatement += values.get(i) + (i== values.size()-1 ? "" : ", ");
            }
            ergoStatement += ").\n";
                        
            ergoOutputFileWriter.write(ergoStatement);                          
            // return a reduced string for the program
            if(stopCounter<300){
                ergoProgram += ergoStatement;
                stopCounter++;
            }else if(stopCounter==300){
                ergoProgram += "...";
                stopCounter++;
            }
        }
        ergoOutputFileWriter.close();
        
        return ergoProgram;
    }
        
    // check if the string is a number
    public static boolean checkNumber(String elem){
        try{
            Double.parseDouble(elem);
            return true;
        }catch(Exception e){
            e.printStackTrace();
        }
        try{
            Integer.parseInt(elem);
            return true;
        }catch(Exception e){
            e.printStackTrace();
        }
        return false;
    }
        
}
