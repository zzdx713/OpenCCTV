/*
 * ResultsAppConnector.hpp
 *
 *  Created on: Apr 7, 2017
 *      Author: anjana
 */

#ifndef RESULTSAPPCONNECTOR_HPP_
#define RESULTSAPPCONNECTOR_HPP_

#include <string>
#include <iostream>
#include <map>

namespace result {
namespace api {

//Error codes
typedef enum
{
	INIT_OK = 0,
	INIT_FAIL,
	AUTH_OK,
	AUTH_FAIL,
	SEND_ANALYTIC_INST_DETAILS_OK,
	SEND_ANALYTIC_INST_DETAILS_FAIL,
	SEND_ANALYTIC_RESULT_OK,
	SEND_ANALYTIC_RESULT_FAIL
} ResultAPICode;

class ResultsAppConnector
{
protected:
	//Status codes
	typedef enum
	{
		INIT_NOT_DONE = 0,
		INIT_DONE,
		AUTH_NOT_DONE,
		AUTH_DONE,
		SEND_ANALYTIC_INST_DETAILS_NOT_DONE,
		SEND_ANALYTIC_INST_DETAILS_DONE,
		SENDING_ANALYTIC_RESULTS,
		//SENDING_ANALYTIC_RESULTS_NOT_DONE
	} ResultAPIStatusCode;

	ResultAPIStatusCode connectorStatus;

public:
	/*
	 * Constructor
	 */
	ResultsAppConnector()
	{
		connectorStatus = INIT_NOT_DONE;
	}

	/*
	 * Purpose: To perform all tasks necessary to initialize the connector
	 *          Usually reading parameters from a XML/YAML/JSON configuration files
	 *          must be performed here. A connector plugin may use a list of input parameters
	 *          provided in the map pointed by pInputParams, a list of input files provided
	 *          in the map pointed by pInputFiles or both.
	 *
	 * Input pInputParams  : A pointer to a map with input parameters need to initialize
	 *                       the plugin. Key = name of the parameter which must be a name returned
	 *                       by the getInputParameterList() function, Value = Value of the
	 *                       parameter as a string.
	 *
	 * Input pInputFiles   : A pointer to a map with input files such as configuration files
	 *                       need to initialize the plugin.
	 *                       Key = name of the file which must be a name returned by the
	 *                       function getInputFileList(). Example : "Configuration file"
	 *                       Value = Absolute file path Example : "\usr\local\anlytic_server\app1\config.xml"
	 *
	 * Output sOutputMsg   : A message indicating the result of executing the function. This could
	 *                       be an error message in case of a failure.
	 */
	virtual ResultAPICode init(const std::map<std::string, std::string>* pInputParams,
			                   const std::map<std::string, std::string>* pInputFiles,
			                   std::string& sOutputMsg)
	{
		//Step 1: Carry out the steps necessary to initialize the connector.

		//Step 2: Set the connector status accordingly
		connectorStatus = INIT_DONE;

		//Step 3: Set the outputs to reflect the results of initializing the connector
		sOutputMsg = "Connector plugin successfully initialized";
		return INIT_OK;
	}

	/*
	 * Purpose: To get authenticated with the remote application who is receiving the analytic
	 *          results. Usually this is called after the init() function. Thus the parameters
	 *          necessary for authentication such as user names, passwords must be read in the
	 *          init() function.
	 *          NOTE : Even if the remote application does not require authentication, implement
	 *          this function and set connectorStatus to AUTH_DONE.
	 *
	 * Output sOutputMsg   : A message indicating the result of executing the function. This could
	 *                       be an error message in case of an authentication failure.
	 */
	virtual ResultAPICode authenticate(std::string& sOutputMsg)
	{
		//Step 1: Check the connector status according to the logic of your connector plugin.
		//        If the init() function is not yet called or init() has failed, trying to
		//        authenticate with the remote application may fail.
		if(connectorStatus == INIT_NOT_DONE)
		{
			return AUTH_FAIL;
		}

		//Step 2: Carry out the steps necessary to authenticate with the remote application

		//Step 3: Set the connector status accordingly
		connectorStatus = AUTH_DONE;

		//Step 4: Set the outputs to reflect the results of authenticating with the remote
		//        application
		sOutputMsg = "Successfully authenticated";
		return AUTH_OK;
	}

	/*
	 * Purpose: Send the details about the analytic instance to the remote results receiving
	 *          application. Usually this is called after the authenticate() function.
	 *          Thus authenticating with the remote application has been already done by
	 *          the time this function is called.
	 *
	 * Input pAnalyticInstanceInfo  : A ptr. to a map with analytic instance information.
	 *                                Key = name of the analytic instance information which
	 *                                must be a name returned by the getAnalyticInstanceInfo()
	 *                                function, Value = Value of the information as a string.
	 *
	 * Output sOutputMsg   : A message indicating the result of executing the function. This could
	 *                       be an error message in case of an authentication failure.
	 *
	 */
	virtual ResultAPICode sendAnalyticInstanceInfo(
			const std::map<std::string, std::string>* pAnalyticInstanceInfo,
			std::string& sOutputMsg)
	{
		//Step 1: Check the connector status according to the logic of your connector plugin.
		//        If init() and authenticate() functions not yet called or init() or
		//        authenticate() has failed, trying to send analytic instance information to
		//        the remote application may fail.
		if(connectorStatus != AUTH_DONE)
		{
			return SEND_ANALYTIC_INST_DETAILS_FAIL;
		}

		//Step 2: Carry out the steps necessary to send the analytic instance information to
		//        the remote application

		//Step 3: Set the connector status accordingly
		connectorStatus = SEND_ANALYTIC_INST_DETAILS_DONE;

		//Step 4: Set the outputs to reflect the results of sending analytic instance information
		//        to the remote application
		sOutputMsg = "Successfully sent analytic instance details";
		return SEND_ANALYTIC_INST_DETAILS_OK;
	}

	/*
	 * Purpose: Send data related to an analytic result along with the images and videos produced
	 *          to the connected analytic results receiving application. Usually this is called
	 *          after the authenticate() function or in some cases after the sendAnalyticInstanceInfo()
	 *          function. Thus authenticating with the remote application has been already done
	 *          by the time this function is called and the remote application is aware about the
	 *          analytic instance executed on the OpenCCTV analytic server.
	 *
	 * Input pData  : A ptr. to a map with analytic data such as timestamp, result text etc. Key = name
	 *
	 * Input pImages: A ptr. to a map of path to analytic results images. Key = filename
	 *
	 * Input pVideos: A ptr. to a map of path to analytic results videos. Key = filename
	 *
	 * Output sOutputMsg   : A message indicating the result of executing the function. This could
	 *                       be an error message in case of an authentication failure.
	 *
	 */
	virtual ResultAPICode sendAnalyticResults(const std::map<std::string, std::string>* pData,
			const std::map<std::string, std::string>* pImages,
			const std::map<std::string, std::string>* pVideos, std::string& sOutputMsg)
	{
		//Step 1: Check the connector status according to the logic of your connector plugin.
		//        If the authenticate() function not yet called or authenticate() has failed,
		//        trying to send analytic results to the remote application may fail. Further
		//        it may be necessary to check that sendAnalyticInstanceInfo() function has
		//        successfully called and the analytic instance information has been already
		//        sent to the remote application.
		if(connectorStatus != SEND_ANALYTIC_INST_DETAILS_DONE)
		{
			return SEND_ANALYTIC_RESULT_FAIL;
		}

		//Step 2: Carry out the steps necessary to send the analytic results to the
		//        remote application

		//Step 3: Set the connector status accordingly
		connectorStatus = SENDING_ANALYTIC_RESULTS;

		//Step 4: Return the outputs to reflect the results of authenticating with the remote
		//        application
		sOutputMsg = "Successfully sent analytic results";
		return SEND_ANALYTIC_RESULT_OK;
	}

	/*
	 * Purpose: Return a list of information about the analytic instance required by the
	 *          remote result receiving application.Override to suit a specific connector
	 *          plugin.
	 *
	 * Return value: An XML stream containing the information about the analytic instance
	 *               formatted as as name, value and required(required = true, optional = false)
	 *               elements
	 */
	virtual std::string getAnalyticInstanceInfo()
	{
		return generateAnalyticInstanceInfoXml();
	}

	/*
	 * Purpose: Return a list of input files required to initialize and communicate with the
	 *          remote results app. Override to suit a specific connector plugin.
	 *
	 * Return value: An XML stream containing the details of input files formatted as name,
	 *               required(required = true, optional = false) and description elements.
	 */
	virtual std::string getInputFileList()
	{
		return generateInputFileListXml();
	}

	/*
	 * Purpose: Return a list of input parameters required to initialize and communicate
	 *          with the remote results application. Override to suit a specific connector plugin.
	 *
	 * Return value: An XML stream containing the details of input parameters formatted as name,
	 *               required(required = true, optional = false) and description elements.
	 */
	virtual std::string getInputParameterList()
	{
		return generateInputParameterList();
	}

	/*
	 * Destructor
	 */
	virtual ~ResultsAppConnector()
	{
	}

private:
	/*
	 * Purpose: Return analytic instance information as an XML stream
	 *          Remove if not used by your connector plugin implementation.
	 */
	std::string generateAnalyticInstanceInfoXml()
	{
		/* Sample XML stream:

			<?xml version="1.0" encoding="UTF-8"?>
			<analyticinstanceinfo>
				<info>
					<name>OpenCCTV Id</name>
					<value>10</value>
					<required>true</required>
				</info>
				<info>
					<name>OpenCCTV Name</name>
					<value>Test Analytic Instance</value>
					<required>true</required>
				</info>
			</analyticinstanceinfo>
		 */

		std::string sXml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
		sXml.append("<analyticinstanceinfo>");
		sXml.append("<info>");
		sXml.append("<name>OpenCCTV Id</name>");
		sXml.append("<value>10</value>");
		sXml.append("<required>true</required>");
		sXml.append("</info>");
		sXml.append("<info>");
		sXml.append("<name>OpenCCTV Name</name>");
		sXml.append("<value>Test Analytic Instance</value>");
		sXml.append("<required>true</required>");
		sXml.append("</info>");
		sXml.append("</analyticinstanceinfo>");
		return sXml;
	}

	/*
	 * Purpose: Return the input file details as an XML stream
	 *          Remove if not used by your connector plugin implementation.
	 */
	std::string generateInputFileListXml()
	{
		/* Sample XML stream:

			<?xml version="1.0" encoding="UTF-8"?>
			<inputfiles>
				<inputfile>
					<name>Configuration file</name>
					<required>true</required>
					<description>
						Include details such as host and port required to communicate with the remote
						application in this file. File format is JSON. A sample file can be
						found in config.json file in the connector plugin archive(.zip file)
					</description>
				</inputfile>

				<inputfiles>
					<name>SSL certificate</name>
					<required>true</required>
					<description>
						The remote application communicates over TLS(SSL).
						Therefore provide the SSL certificate file of the remote application.
					</description>
				</inputfile>
			</inputfiles>
		 */

		std::string sXml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
		sXml.append("<inputfiles><inputfile>");
		sXml.append("<name>Configuration file</name>");
		sXml.append("<required>true</required>");
		sXml.append("<description>");
		sXml.append("Include details such as host and port required to communicate with the remote");
		sXml.append("application in this file. File format is JSON. A sample file can be");
		sXml.append("found in config.json file in the connector plugin archive(.zip file)");
		sXml.append("</description></inputfile>");

		sXml.append("<inputfile><name>SSL certificate</name>");
		sXml.append("<required>true</required>");
		sXml.append("<description>");
		sXml.append("The remote application communicates over TLS(SSL).");
		sXml.append("Therefore provide the SSL certificate file of the remote application.");
		sXml.append("</description></inputfile>");

		sXml.append("</inputfiles>");
		return sXml;
	}

	/*
	 * Purpose: Return the input parameter details as an XML stream
	 *          Remove if not used by your connector plugin implementation.
	 */
	std::string generateInputParameterList()
	{
		/* Sample XML stream:

			<?xml version="1.0" encoding="UTF-8"?>
			<inputparams>
				<inputparam>
					<name>Host URL</name>
					<required>true</required>
					<description>
						URL of the remote results application host
					</description>
				</inputparam>
				<inputparam>
					<name>Port number</name>
					<required>false</required>
					<description>
						Port of the remote results application host
					</description>
				</inputparam>
			</inputparams>
		*/

		std::string sXml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
		sXml.append("<inputparams><inputparam>");
		sXml.append("<name>Host URL</name>");
		sXml.append("<required>true</required>");
		sXml.append("<description>");
		sXml.append("URL of the remote results application host");
		sXml.append("</description></inputparam>");

		sXml.append("<inputparam><name>Port number</name>");
		sXml.append("<required>false</required>");
		sXml.append("<description>");
		sXml.append("Port of the remote results application host");
		sXml.append("</description></inputparam>");

		sXml.append("</inputparams>");
		return sXml;
	}
};

} // namespace api
} // namespace analytic

/*
 * In YOUR_CONNECTOR_CLASS.hpp include these two functions.
 *
extern "C" YOUR_CONNECTOR_CLASS* create() {
    return new YOUR_CONNECTOR_CLASS();
}

extern "C" void destroy(YOUR_CONNECTOR_CLASS* p) {
    if(p) {
    	// do whatever you want to close everything
    	delete p;
    }
}
*/

#endif /* RESULTSAPPCONNECTOR_HPP_ */

