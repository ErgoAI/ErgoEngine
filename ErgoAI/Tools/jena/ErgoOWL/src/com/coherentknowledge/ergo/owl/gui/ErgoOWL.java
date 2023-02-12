/*
 * Copyright 2014, Coherent Knowledge Systems.
 *
 * All rights reserved.
 */

package com.coherentknowledge.ergo.owl.gui;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Scanner;
import java.util.Set;

import com.hp.hpl.jena.rdf.model.Model;
import com.hp.hpl.jena.util.FileManager;

/**
 *
 * @author Paul Fodor
 */
public class ErgoOWL {

    // flag to select between triples and quads
    static boolean triplesFlag = true;
        
    // flag to select between fastload output format and Ergo output
    //  fastload = 1
    //  predicates = 2
    //  frames = 3
    static int outputFormat = 1;

    // iris2links: xsd -> http://www.w3.org/2001/XMLSchema#, ... 
    public static HashMap<String, String> iris2links;
    public static ArrayList<String> irisArrayList = new ArrayList<String>();
        
    /**
     * initializeIris
     */
    public static void initializeIris() {
        irisArrayList = new ArrayList<String>();
        iris2links = new HashMap<String, String>();
        // default IRIs
        irisArrayList.add("xsd");
        iris2links.put("xsd", "http://www.w3.org/2001/XMLSchema#");
        irisArrayList.add("rdf");
        iris2links.put("rdf", "http://www.w3.org/1999/02/22-rdf-syntax-ns#");
        irisArrayList.add("rdfs");
        iris2links.put("rdfs", "http://www.w3.org/2000/01/rdf-schema#");
        irisArrayList.add("owl");
        iris2links.put("owl", "http://www.w3.org/2002/07/owl#");
        /*
          irisArrayList.add("err");
          iris2links.put("err", "http://www.w3.org/2005/xqt-errors#");
          irisArrayList.add("fn");
          iris2links.put("fn", "http://www.w3.org/2005/xpath-functions#");
          irisArrayList.add("foaf");
          iris2links.put("foaf", "http://xmlns.com/foaf/0.1/");
          irisArrayList.add("fti");
          iris2links.put("fti", "http://franz.com/ns/allegrograph/2.2/textindex/");
          irisArrayList.add("skos");
          iris2links.put("skos", "http://www.w3.org/2004/02/skos/core#");
          irisArrayList.add("dc");
          iris2links.put("dc", "http://purl.org/dc/elements/1.1/");
          irisArrayList.add("dcterms");
          iris2links.put("dcterms", "http://purl.org/dc/terms/");
        */
    }

    /**
     * setIris
     *   the argument is a string with "iri = link" lines
     */
    public static void setIris(String irisText) {
        iris2links = new HashMap<String, String>();

        String[] iris = irisText.split("\n");
                
        for(int i=0; i<iris.length; i++){
            String[] oneIri = iris[i].split("=");
            try{
                String iri = oneIri[0].trim();
                String link = oneIri[1].trim();
                iris2links.put(iri, link);
            }catch(Exception e){
                e.printStackTrace();
                System.out.println("Incorrect IRI: " + iris[i]);
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
            irisTextAreaString += iri + " = " + iriLink + "\n";
        }
                
        return irisTextAreaString;
    }

    /**
     * replaceLinks2IRIs
     * 
     * @param value that may contain an IRI link
     * @return
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
                    returnValue = "__fCURI__(" + iri + "," + subValue + ")";
                    return returnValue;
                }else if(outputFormat == 2 || outputFormat == 3){
                    returnValue = iri + "#" + value.substring(endPos);
                    return returnValue;
                }       
            }
        }
        // if no IRI was found, return the same string
        return value;
    }

    /**
     * parseNQuads
     * 
     * @param nQuad
     * @return
     */
    public static ArrayList<String> parseNQuads(String nQuad) {
        ArrayList<String> returnValueArrayList = new ArrayList<String>();
        String rem = nQuad;
        rem = rem.trim();
        // extract elements in nquad
        while(!rem.isEmpty() && !rem.equals(".")){
            String elem = "";
            // see if the first argument is a link
            if(rem.charAt(0) == '<'){
                elem = rem.substring(1, rem.indexOf('>'));
                String oldElem = elem;
                elem = replaceLinks2IRIs(elem);
                // Put single quotes around the link only if a prefix is not created 
                if(oldElem.equals(elem) && !elem.startsWith("__fCURI__")) 
                    elem = "'" + elem + "'";
                // prepare rem for the next iteration
                rem = rem.substring(rem.indexOf('>') + 2); // also ignore space
            }
            // see if the first argument is a string
            else if(rem.charAt(0) == '"'){ 
                // the string may contain escaped double quotes: \"
                // if the quote that I found is a escaped double quote \", then we continue searching 
                // for the next double quote that is not an escape double quote \"
                int nextPos = 1;
                int nextQuotesPos = rem.indexOf('"', 1);
                int escapeQuotesPos = rem.indexOf("\\\"", 1);
                while(nextQuotesPos == escapeQuotesPos + 1){
                    nextPos = rem.indexOf("\\\"", nextPos) + 2;
                    nextQuotesPos = rem.indexOf('"', nextPos);
                    escapeQuotesPos = rem.indexOf("\\\"", nextPos);
                }
                // finaly, I extract the string:
                elem = rem.substring(0, nextQuotesPos + 1);
                // extract the remainder string
                rem = rem.substring(nextQuotesPos + 1); // also ignore space

                // eliminate start and end double quotes
                elem = elem.substring(1, elem.length()-1);

                // eliminate the type: ^^type in the remainder
                if(rem.charAt(0)=='^'){
                    int typeEndPos = rem.indexOf(">");
                    rem = rem.substring(typeEndPos + 1);
                    // extract numbers
                    if(checkNumber(elem))
                        elem = elem;
                    else{ // replace double quotes with single quotes
                        if(elem.contains("'"))
                            elem = elem.replaceAll("'", "''");
                        elem = "'" + elem + "'";
                    }
                }else{
                    // replace double quotes with single quotes
                    if(elem.contains("'"))
                        elem = elem.replaceAll("'", "''");
                    elem = "'" + elem + "'";
                    // escape Unicode characters
                    if(elem.indexOf("\\u")>=0){
                        int pos = elem.indexOf("\\u");
                        while(pos>0 && elem.length() >= pos+5){
                            elem = elem.substring(0,pos+6) + "\\" + elem.substring(pos+6);
                            pos = elem.indexOf("\\u",pos+5);
                        }
                        //System.out.println("Debug: " + elem);
                    }                                   
                }
            }
            // see if the first argument is an atom
            else if(rem.charAt(0) == '\''){ 
                elem = rem.substring(0, rem.indexOf('\'', 1) + 1);
                rem = rem.substring(rem.indexOf('\'', 1) + 1); // also ignore space
            }
            // is just separated by comma
            else {
                //System.out.println("Debug: <" + rem + ">");
                elem = rem.substring(0, rem.indexOf(' '));
                //System.out.println("Debug: " + elem + "");
                rem = rem.substring(rem.indexOf(' ') + 1); // also ignore space
            }
            // if the elem starts with _, then put the elem in single quotes
            if(elem.startsWith("_") && !elem.startsWith("__fCURI__"))
                elem = "'" + elem + "'";
            returnValueArrayList.add(elem);
            rem = rem.trim();
        }

        return returnValueArrayList;
    }
        
    public static boolean checkNumber(String elem){
        //if(elem.equals("1"))
        //      System.out.println("Debug");
        try{
            Double.parseDouble(elem);
            // my version of parseDouble
            //for(int i=1; i<elem.length()-1; i++)
            //  if( !(('0'<=elem.charAt(i) && elem.charAt(i)<='9') || elem.charAt(i)=='.') )
            //          return false;
            return true;
        }catch(Exception e){
        }
        try{
            Integer.parseInt(elem);
            return true;
        }catch(Exception e){
        }
        return false;
    }

    /**
     * 
     * @param rdfXmlFile
     * @param defaultGraphName
     * @return
     * @throws IOException 
     */
    public static String importRDFXML(File rdfXmlFile, String defaultGraphName) throws IOException {
        String rdfXmlFileAbsolutePath = rdfXmlFile.getAbsolutePath();
                
        // load the OWL file in Jena and translate it into n-triples
        Model model = FileManager.get().loadModel(rdfXmlFile.toURI().toString());
        InputStream in = FileManager.get().open(rdfXmlFileAbsolutePath);
        model.read(in, "");
        // output .nt file
        String ntriplesFileAbsolutePath = rdfXmlFileAbsolutePath + ".nt";
        File ntriplesFile = new File(ntriplesFileAbsolutePath);
        try {
            OutputStream ntriplesFileOutputStream = new FileOutputStream(ntriplesFile);
            model.write(ntriplesFileOutputStream, "N-TRIPLES");
            ntriplesFileOutputStream.close();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }

        // output format: fastload or Ergo
        String ergoOutputFileAbsolutePath;
        if(outputFormat == 1)
            ergoOutputFileAbsolutePath = rdfXmlFileAbsolutePath + ".P";
        else
            ergoOutputFileAbsolutePath = rdfXmlFileAbsolutePath + ".ergo";
                
        // read the n-triples file and generate the Ergo translation
        return importRDFNQuads(ntriplesFile, ergoOutputFileAbsolutePath, defaultGraphName);
    }

    public static String importRDFNQuads(File ntriplesFile, String defaultGraphName) throws IOException {
        String ntriplesFileAbsolutePath = ntriplesFile.getAbsolutePath();
        String ergoOutputFileAbsolutePath;
        if(outputFormat == 1)
            ergoOutputFileAbsolutePath = ntriplesFileAbsolutePath + ".P";
        else
            ergoOutputFileAbsolutePath = ntriplesFileAbsolutePath + ".ergo";
        return importRDFNQuads(ntriplesFile, ergoOutputFileAbsolutePath, defaultGraphName);
    }
        
    public static String importRDFNQuads(File ntriplesFile, String ergoOutputFileAbsolutePath, String defaultGraphName) throws IOException {
        // add the IRIs in the .ergo file prefix
        String ergoPrefix = "";
                
        if(outputFormat != 1){
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
        }else{ // outputFormat is 2 or 3
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
                ergoPrefix += "#deffast   " + iri + " " + link + "\n";
            }
                        
            // add a comment in the .ergo file prefix before the translation
            ergoPrefix += "\n% imported OWL axioms\n";
        }

        // collect the output for the return to the TextArea
        String ergoProgram = ergoPrefix;
        int stopCounter = 0; // stop the collection of the text after 100 lines
                
        FileWriter ergoOutputFileWriter = null;
                
        try {
            // read the n-triples file and generate the Ergo translation
            Scanner ntriplesFileScanner = new Scanner(ntriplesFile);

            // write the output in the .ergo file
            File ergoOutputFile = new File(ergoOutputFileAbsolutePath);
            ergoOutputFileWriter = new FileWriter(ergoOutputFile);
            ergoOutputFileWriter.write(ergoPrefix);
                        
            String ergoStatement;
                        
            // read one triple line at a time
            while (ntriplesFileScanner.hasNext()) {
                String nQuad = ntriplesFileScanner.nextLine();
                // debug
                // System.out.println("Debug: " + nQuad);
                // parse nTriple
                ArrayList<String> values = parseNQuads(nQuad);
                // add the default graph name if the quad is only a triple
                if(values.size() == 3){
                    values.add("'" + defaultGraphName + "'");
                }
                // ToDo: collect import statements for multiple files - e.g., gist
                /*
                  if (values[0].equals("owl#imports")) {
                  String includedFile = extract(x[3]).substring(
                  extract(x[3]).lastIndexOf("/") + 1,
                  extract(x[3]).length() - 1);
                  ergoProgram.append( "#include \"" + includedFile + ".nq.ergo\"\n" );
                  }
                */
                // serialize the output
                if(values.size() == 4){ // good n-quads
                                        //System.out.println("Debug " + triplesFlag);
                    if(outputFormat == 3){ // frames
                        ergoStatement =  values.get(0) + "[" + values.get(1) + " -> " + values.get(2) + "].\n";
                    }else if(triplesFlag) // n-triples
                        ergoStatement =  values.get(1) + "(" + values.get(0) + ", " + values.get(2) + ").\n";
                    else // n-quads
                        ergoStatement =  values.get(1) + "(" + values.get(0) + ", " + values.get(2) + ", " + values.get(3) + ").\n";
                                        
                    // write the ergo statement in the file 
                    ergoOutputFileWriter.write(ergoStatement);
                                        
                    // return a reduced string for the program
                    if(stopCounter<300){
                        ergoProgram += ergoStatement;
                        stopCounter++;
                    }else if(stopCounter==300){
                        ergoProgram += "...";
                        stopCounter++;
                    }
                }else{
                    System.out.println("Debug: an incorrect triple is: " + values);
                    throw new IOException("Incorrect triple value read.");
                }
            }
        } catch (FileNotFoundException e) {
            //e.printStackTrace();
            // rethrow the exception to let the caller know that there was an error
            throw e;
        } catch (IOException e) {
            //e.printStackTrace();
            // rethrow the exception to let the caller know that there was an error
            throw e;
        } finally{
            try{ if(ergoOutputFileWriter!=null) ergoOutputFileWriter.close(); } catch(Exception ignore){ }; 
        }
                
        return ergoProgram;
    }

    public static String importRDFJSON(File jsonFile, String defaultGraphName) throws IOException {
        String jsonFileAbsolutePath = jsonFile.getAbsolutePath();
                
        // load the OWL file in Jena and translate it into n-triples
        Model model = FileManager.get().loadModel(jsonFile.toURI().toString());
        InputStream in = FileManager.get().open(jsonFileAbsolutePath);
        model.read(in, null, "JSON-LD");
        // output .nt file
        String ntriplesFileAbsolutePath = jsonFileAbsolutePath + ".nt";
        File ntriplesFile = new File(ntriplesFileAbsolutePath);
        try {
            OutputStream ntriplesFileOutputStream = new FileOutputStream(ntriplesFile);
            model.write(ntriplesFileOutputStream, "N-TRIPLES");
            ntriplesFileOutputStream.close();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }

        // output format: fastload or Ergo
        String ergoOutputFileAbsolutePath;
        if(outputFormat == 1)
            ergoOutputFileAbsolutePath = jsonFileAbsolutePath + ".P";
        else
            ergoOutputFileAbsolutePath = jsonFileAbsolutePath + ".ergo";
                
        // read the n-triples file and generate the Ergo translation
        return importRDFNQuads(ntriplesFile, ergoOutputFileAbsolutePath, defaultGraphName);
    }

    public static String importRDFTTL(File ttlFile, String defaultGraphName) throws IOException {
        String ttlFileAbsolutePath = ttlFile.getAbsolutePath();
                
        // load the OWL file in Jena and translate it into n-triples
        Model model = FileManager.get().loadModel(ttlFile.toURI().toString());
        InputStream in = FileManager.get().open(ttlFileAbsolutePath);
        model.read(in, null, "TTL");
        // output .nt file
        String ntriplesFileAbsolutePath = ttlFileAbsolutePath + ".nt";
        File ntriplesFile = new File(ntriplesFileAbsolutePath);
        try {
            OutputStream ntriplesFileOutputStream = new FileOutputStream(ntriplesFile);
            model.write(ntriplesFileOutputStream, "N-TRIPLES");
            ntriplesFileOutputStream.close();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }

        // output format: fastload or Ergo
        String ergoOutputFileAbsolutePath;
        if(outputFormat == 1)
            ergoOutputFileAbsolutePath = ttlFileAbsolutePath + ".P";
        else
            ergoOutputFileAbsolutePath = ttlFileAbsolutePath + ".ergo";
                
        // read the n-triples file and generate the Ergo translation
        return importRDFNQuads(ntriplesFile, ergoOutputFileAbsolutePath, defaultGraphName);
    }
}
