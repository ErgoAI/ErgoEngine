/*
 * Copyright 2015-2016, Coherent Knowledge Systems.
 *
 * Author: Paul Fodor
 *
 * All rights reserved.
 */

package com.coherentknowledge.ergo.owl;

import java.io.File;
import java.io.FileWriter;
import java.io.InputStream;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Set;

import org.apache.jena.riot.RDFDataMgr;
import org.apache.log4j.Logger;

import com.hp.hpl.jena.rdf.model.Literal;
import com.hp.hpl.jena.rdf.model.Model;
import com.hp.hpl.jena.rdf.model.ModelFactory;
import com.hp.hpl.jena.rdf.model.Property;
import com.hp.hpl.jena.rdf.model.RDFNode;
import com.hp.hpl.jena.rdf.model.Statement;
import com.hp.hpl.jena.rdf.model.StmtIterator;
import com.hp.hpl.jena.util.FileManager;

//import org.apache.commons.io.IOUtils;

/**
 * The ErgoOWLAPI class is used by the Ergo API to load RDF and OWL files.
 *
 * @author Paul Fodor
 */
public class ErgoOWLAPI {

    private static Logger logger = Logger.getLogger("ErgoOWL");

    /*
     * Using constructor in order to initialize logger.
     */
    public ErgoOWLAPI() {
        // Logger.getRootLogger().setLevel(Level.OFF);
    }

    // flag to select between fastload output format or Ergo output:
    // fastload (default)
    // predicates
    // frames
    // rdf predicate
    private static String outputFormat = "fastload";

    // iris2links: xsd -> http://www.w3.org/2001/XMLSchema#, ...
    private static HashMap<String, String> iris2links;
    // the list of all the iris
    private static Set<String> irisSet = new HashSet<String>();

    /**
     * sets the default iris (called if the iris is the empty string)
     * 
     * returns void because it just sets the static translation IRIs
     */
    private static void defaultIris() {
        irisSet = new HashSet<String>();
        iris2links = new HashMap<String, String>();
        // default IRIs
        irisSet.add("xsd");
        iris2links.put("xsd", "http://www.w3.org/2001/XMLSchema#");
        irisSet.add("rdf");
        iris2links.put("rdf", "http://www.w3.org/1999/02/22-rdf-syntax-ns#");
        irisSet.add("rdfs");
        iris2links.put("rdfs", "http://www.w3.org/2000/01/rdf-schema#");
        irisSet.add("owl");
        iris2links.put("owl", "http://www.w3.org/2002/07/owl#");
    }

    /**
     * sets the iris to the ones passed as input by the user. If the iris are
     * empty, it uses the default IRIs xsd, rdf, rdfs and owl.
     * 
     * @param the
     *            argument is a string with "iri = link" per line
     * 
     * @return void if it succeeded or an Exception if some of the IRIs were
     *         incorrect
     */
    private static void setIris(String iriPrefixes) throws Exception {
        // set the default Ergo IRIs
        defaultIris();
        // parse the input iris
        String[] iris = iriPrefixes.split("\n");
        for (int i = 0; i < iris.length; i++) {
            // if an iri is not an empty line
            if (!iris[i].trim().equals("")) {
                try {
                    String[] oneIri = iris[i].split("=");
                    String iri = oneIri[0].trim();
                    String link = oneIri[1].trim();
                    // if the value is already in the HashMap, we replace the
                    // old
                    // value
                    iris2links.put(iri, link);
                    // If the irisSet already contains the element, the call
                    // leaves
                    // the set unchanged
                    irisSet.add(iri);
                } catch (Exception e) {
                    // if the iri is not defined correctly, we are going to throw an exception 
                    logger.error("Incorrect IRI: " + iris[i], e);
                    throw new Exception("Incorrect IRI: " + iris[i]);
                }
            }
        }
    }

    /**
     * replaces a link with an IRI and returns the output in the desired output
     * format.
     * 
     * @param url
     *            is a URL String that may contain an IRI
     * @return the string that contains the IRI or the original string
     */
    private static String replaceLinksWithIRIs(String url) throws Exception {
        String iriLink;
        String returnValue = url;
        // for every iri-link check if it appears in the current URL
        for (String iri : irisSet) {
            iriLink = iris2links.get(iri);
            if (url.contains(iriLink)) {
                int endPos = url.indexOf(iriLink) + iriLink.length();
                // output format:
                if (outputFormat.equals("fastload")) {
                    String subValue = url.substring(endPos);
                    // prevent macro-expansion of the 2nd arg (local name)
                    // of the __fCURI__ macro: uses  __freeze__
                    returnValue = "__fCURI__(" + iri + ", __freeze__ " + subValue + ")";
                    return returnValue;
                } else if (outputFormat.equals("predicates") || outputFormat.equals("frames")) {
                    returnValue = iri + "#" + url.substring(endPos);
                    return returnValue;
                } else {
                    Exception e = new Exception("invalid output format");
                    logger.error("Incorrect output format, IRI: " + iriLink, e);
                    throw e;
                }
            }
        }
        // if no known IRI was found, return the same string
        // the __freeze__ prefix prevents macro-expansion of the URL
        if (outputFormat.equals("fastload")) {
            return "__fIRI__(__freeze__ " + url + ")";
        } else {
            return "'" + url + "'";
        }
    }

    private static String RDFNode2String(RDFNode node) throws Exception {
        if (node == null)
            return "'NULL'(_)";
        if (node.isURIResource()) {
            return replaceLinksWithIRIs(node.toString());
        } else if (node.isAnon()) {
            return "'" + node.toString().replace("'", "''") + "'";
        } else if (node.isLiteral()) {
            Literal literal = node.asLiteral();
            String lexical = literal.getLexicalForm().replace("'", "''");
            String typeuri = literal.getDatatypeURI();

            // this enables the use of the fast type comparison using ==
            if (typeuri != null)
                typeuri = typeuri.intern();
            else
                return "'" + node.toString().replace("'", "''") + "'";

            switch (typeuri) {
            case "http://www.w3.org/2001/XMLSchema#string":
                return "__fSTR__(\"" + lexical.replaceAll("\"", "") + "\")";
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
            case "http://www.w3.org/2001/XMLSchema#time":
                return "__fTIME__(" + lexical + ")";
            case "http://www.w3.org/2001/XMLSchema#date":
                return "__fDATE__(" + lexical + ")";
            case "http://www.w3.org/2001/XMLSchema#dtime":
                return "__fDTIME__(" + lexical + ")";
            case "http://www.w3.org/2001/XMLSchema#duration":
                return "__fDURATION__(" + lexical + ")";
            case "http://www.w3.org/2001/XMLSchema#boolean":
                return "__fBOOL(" + lexical + ")";
            default:
                return "unknown"; //"__fUNKNOWN__('" + typeuri + "\",\"" + lexical + "\")";
            }
        } else{
            // if it is not a literal and not anon
            // atom
            return "'" + node.toString().replace("'", "''") + "'";
        }
    }

    /**
     * Method to translate nquads into Ergo.
     * 
     * @return void if the translation succeeded or an Exception if it failed
     */
    private static void importRDF(String inputFileName,
                                  String inputLangSyntax,
                                  String outFileName) throws Exception {
        try {
            // Add the IRIs in the .ergo file
            String ergoPrefix = "";
            // predicate or frame output syntax
            if (outputFormat.equals("predicates") || outputFormat.equals("frames")) {
                ergoPrefix = "/* Ergo Built-in prefixes used:\n"
                    + "xsd 'http://www.w3.org/2001/XMLSchema#'\n"
                    + "rdf 'http://www.w3.org/1999/02/22-rdf-syntax-ns#'\n"
                    + "rdfs 'http://www.w3.org/2000/01/rdf-schema#'\n"
                    + "owl 'http://www.w3.org/2002/07/owl#'\n" + "*/\n\n"
                    + "/* Other prefixes used:  */\n";
                // add the other IRIs to the ergoPrefix
                for (String iri : irisSet) {
                    // add if it is not one of the default prefixes
                    if (!iri.equals("xsd") && !iri.equals("rdf")
                        && !iri.equals("rdfs") && !iri.equals("owl"))
                        ergoPrefix += ":- iriprefix{" + iri + " = '"
                            + iris2links.get(iri) + "'}.\n";
                }
                // add a comment in the .ergo file prefix before the translation
                ergoPrefix += "\n/* imported OWL axioms -- these are in the form of fact triples */\n";
            } else {
                // fastload output syntax
                ergoPrefix = "";
                /*
                 * "#mode save\n" + "#mode nostring   \"\\!#'\"\n" +
                 * "#define __fCURI__(X,Y)   'i\\x08\\#1#2'\n" +
                 * "#mode restore\n\n";
                 */
                // add the IRIs to the fastload .P file
                for (String iri : irisSet) {
                    String link = iris2links.get(iri);
                    ergoPrefix += "#deffast   " + iri + " " + link + "\n";
                }
                // add a comment in the .ergo file prefix before the translation
                ergoPrefix += "\n#mode nocomment \"%\"\n";
                ergoPrefix += "\n% imported OWL axioms\n";
            }
            
            // build the output Ergo file
            String ntriplesFileAbsolutePath =
                (new File(inputFileName)).getAbsolutePath();
            String ergoOutputFileAbsolutePath =
                (new File(outFileName)).getAbsolutePath();

            /*
             * Translate the triples: read the n-triples file and generate the
             * Ergo translation
             */
            Model model = ModelFactory.createDefaultModel();
            InputStream in;

            if (inputLangSyntax.equals("")) {
                // try to guess the type of the file from the extension
                String fileExtension = ntriplesFileAbsolutePath.substring(ntriplesFileAbsolutePath.lastIndexOf('.'));
                switch(fileExtension){
                case ".ttl":
                    model.read(ntriplesFileAbsolutePath, "TURTLE");
                    break;
                case ".rdf":
                case ".owl":
                	//in = RDFDataMgr.open(ntriplesFileAbsolutePath);
                    model = FileManager.get().loadModel(new File(ntriplesFileAbsolutePath).toURI().toString());
                    in = FileManager.get().open(ntriplesFileAbsolutePath);
                    model.read(in, "");
                    break;
                case ".jsonld":
                    model.read(ntriplesFileAbsolutePath, "JSON-LD");
                    break;
                case ".nq":
                    RDFDataMgr.read(model, ntriplesFileAbsolutePath);
                    break;
                default:
                    model.read(ntriplesFileAbsolutePath);
                    break;
                }
            } else if (!inputLangSyntax.equals("RDF/XML")) {
                model.read(ntriplesFileAbsolutePath, inputLangSyntax);
            } else {
                model = FileManager.get().loadModel(new File(ntriplesFileAbsolutePath).toURI().toString());
                in = FileManager.get().open(ntriplesFileAbsolutePath);
                model.read(in, "");
            }
            
            File ergoOutputFile = new File(ergoOutputFileAbsolutePath);
            //FileWriter ergoOutputFileWriter = new FileWriter(ergoOutputFile);
            
            java.io.PrintWriter ergoOutputFileWriter = new java.io.PrintWriter(ergoOutputFile,"UTF-8");
            // write the prefix:
            ergoOutputFileWriter.write(ergoPrefix);

            StmtIterator iteratorForProperties = model.listStatements();
            // Jena has no way to get the list of all properties, but we would
            // like to have the statements grouped by property
            Set<Property> properties = new HashSet<Property>();
            while (iteratorForProperties.hasNext()) {
                Statement statement = iteratorForProperties.nextStatement();
                properties.add(statement.getPredicate());
            }
            
            // iterate over the Properties and collect the Statements
            for (Property property : properties) {
                StmtIterator iterator = model.listStatements();
                while (iterator.hasNext()) {
                    Statement statement = iterator.nextStatement();
                    // take the statements with the same property 
                    if (statement.getPredicate().equals(property)) {
                        String ergoStatement;
                        // get the subject
                        String subject = statement.getSubject().getURI();
                        if (subject == null) { // Blank node
                            subject = "'" + statement.getSubject().toString()
                                + "'";
                        } else {
                            subject = replaceLinksWithIRIs(subject);
                        }
                        // get the predicate
                        String predicate = statement.getPredicate().getURI();
                        if (predicate == null) { // Blank node
                            predicate = "'"
                                + statement.getPredicate().toString() + "'";
                        } else {
                            predicate = replaceLinksWithIRIs(predicate);
                        }
                        // get the object
                        RDFNode objectNode = statement.getObject();
                        String object = objectNode.toString();
                        if (objectNode.isURIResource()) {
                            object = replaceLinksWithIRIs(object);
                        } else {
                            object = RDFNode2String(objectNode);
                        }
                        // serialize in the output file
                        if (outputFormat.equals("fastload") || outputFormat.equals("predicates")) {
                            /*
                             * fastload or predicate output
                             */
                              ergoStatement = predicate + "(" + subject
                                    + ", " + object + ").\n";
                        } else if (outputFormat.equals("frames")) {
                             ergoStatement =
                                    subject + "["
                                    + predicate + " -> " + object
                                    + "].\n";
                        } else {
                            // this should not happen, since we are guarding
                            // on the prolog side
                            throw new Exception(
                                                "ErgoOWL(load): invalid output format, " + outputFormat);
                        }
                        // write the ergo statement into the output file
                        ergoOutputFileWriter.write(ergoStatement);
                    }
                }
            }
            // close the output writer
            ergoOutputFileWriter.close();
        } catch (Exception e) {
            //e.printStackTrace();
            logger.error(e);
            // re-throw the exception to let the caller know about it
            throw e;
        }
    }

    /**
     * Method to translate an input file into Ergo.
     * 
     * @return boolean true if the translation succeeded or an exception if it
     *         failed
     */
    public static boolean translateRDF(String inputFileName,
                                       String inputLangSyntax,
                                       String outFileName,
                                       String outputFormat,
                                       String iriPrefixes) throws Exception {
        try {
            ErgoOWLAPI.outputFormat = outputFormat;
            // set IRIs Hashmap and ArrayList
            setIris(iriPrefixes);
            // call the right translator for the inputLangSyntax
            importRDF(inputFileName, inputLangSyntax.toUpperCase(), outFileName);
            // the translation was successful
            return true;
        } catch (Exception e) {
            logger.error(e);
            throw e;
            //throw new Exception("ErgoOWL error: " + e.getMessage());
            // throw new Exception("ErgoOWL(load): " + " inputFileName="
            // + inputFileName + ", syntax=" + inputLangSyntax
            // + ", triplesFlag=" + triplesFlag + ", outputFormat="
            // + outputFormat + ", iriPrefixes=" + iriPrefixes);
        }
        //return false;
    }

    public static void main(String[] args) {
        // ErgoOWLAPI_Tests.main(args);
    }

}
