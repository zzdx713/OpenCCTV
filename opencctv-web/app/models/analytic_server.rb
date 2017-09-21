class AnalyticServer < ActiveRecord::Base
  validates :name, presence: true, uniqueness: true
  validates :ip, presence: true
  validates :port, presence: true
  validates :port, uniqueness: { scope: :ip }

  has_many :analytic_instances, dependent: :restrict_with_exception

=begin
  def send_to_server(message_type)
    msg_details = nil
    reply = nil
    error_reply = reply = "<?xml version=\"1.0\" encoding=\"utf-8\"?><analyticreply><operation>AnalyticServerStatus</operation><status>Unknown</status><pid>0</pid></analyticreply>"

    begin
      context = ZMQ::Context.new(1)
      requester = nil

      if context
        #Create a request type socket
        requester = context.socket(ZMQ::REQ)

        #Set the socket options
        requester.setsockopt(ZMQ::SNDTIMEO, 10000) # A 10 second timeout is set for send
        requester.setsockopt(ZMQ::RCVTIMEO, 10000) # A 10 second timeout is set for receive
        requester.setsockopt(ZMQ::LINGER,0)

        #Connects to the socket
        rc = requester.connect("tcp://#{self.ip}:#{self.port}")
        message = "<?xml version=\"1.0\" encoding=\"utf-8\"?><analyticrequest><operation>#{message_type}</operation></analyticrequest>"

        #Send a request
        rc = requester.send_string(message) unless zmq_error_check(rc)

        #Receive reply
        rc = requester.recv_string(reply) unless zmq_error_check(rc)
      end

      #Close the socket and terminate the ZMQ context
      requester.close unless requester.nil?
      context.terminate
    rescue
      reply = nil
    end

    reply.nil? ? msg_details = parse_reply(error_reply) : msg_details = parse_reply(reply)
    return msg_details
  end

  private
  def parse_reply (xml_string)
    #Example reply messages :
    #successfull : <?xml version="1.0" encoding="utf-8"?><analyticreply><operation>analyticserverstatus</operation><status>Running</status><pid>27234</pid></analyticreply>
    #failed : <?xml version="1.0" encoding="utf-8"?><analyticreply><operation>analyticserverstatus</operation><status>Unknown</status><pid>0</pid></analyticreply>
    server_status = nil
    server_pid = nil

    msg_details = {}
    if(!xml_string.nil? && xml_string.start_with?("<"))
      begin # XML parsing
        doc = Nokogiri::XML(xml_string)
        server_status = doc.xpath('//analyticreply//status')[0].content.to_s.strip
        server_pid = doc.xpath('//analyticreply//pid')[0].content.to_s.strip
      rescue # Handle XML parsing errors
        server_status = 'Unknown' if server_status.nil?
        server_pid = '0' if server_pid.nil?
      end # End XML parsing
    end

    msg_details[:server_status] = server_status
    msg_details[:server_pid] = server_pid

    return msg_details
  end

  def zmq_error_check(rc)
    if ZMQ::Util.resultcode_ok?(rc)
      false
    else
      STDERR.puts "Operation failed, errno [#{ZMQ::Util.errno}] description [#{ZMQ::Util.error_string}]"
      true
    end
  end
=end
end
